#![allow(warnings)] //TODO

use {
    std::sync::Arc,
    futures::future::FutureExt as _,
    iced::{
        Command,
        pure::{
            Application,
            Element,
            widget::{
                Button,
                Row,
                Text,
            },
        },
    },
    iced_native::command::Action,
    pyo3::{
        prelude::*,
        types::PyDict,
    },
    rfd::AsyncFileDialog,
    tokio::task::spawn_blocking,
};

#[pyclass(dict)]
#[derive(Debug, Clone, Copy)]
struct Window;

#[pymethods]
impl Window {
    #[new] fn new() -> Self { Self }
    fn update_status(&self, text: &str) { let _ = text; } //TODO
    fn update_progress(&self, val: f64) { let _ = val; } //TODO
}

#[derive(Debug, thiserror::Error)]
enum Error {
    #[error(transparent)] Iced(#[from] iced::Error),
    #[error(transparent)] Python(#[from] PyErr),
    #[error(transparent)] Task(#[from] tokio::task::JoinError),
}

#[derive(Debug, Clone)]
enum Message {
    Done,
    Error(Arc<Error>),
    Generate,
    SelectOutDir {
        settings: Py<PyAny>,
        window: Window,
        spoiler: Py<PyAny>,
        rom: Py<PyAny>,
    },
}

#[derive(Default)]
struct Gui {
    error: Option<Arc<Error>>,
    generating: bool,
}

impl Application for Gui {
    type Executor = iced::executor::Default;
    type Message = Message;
    type Flags = ();

    fn new((): ()) -> (Self, Command<Message>) { (Self::default(), Command::none()) } //TODO move Python version check here
    fn title(&self) -> String { format!("OoT Randomizer") }

    fn update(&mut self, msg: Message) -> Command<Message> {
        match msg {
            Message::Done => self.generating = false,
            Message::Error(e) => self.error = Some(e),
            Message::Generate => {
                self.generating = true;
                //TODO if goal hints are used and there are more than 5 worlds, ask for confirmation due to long generation times
                //TODO when generating a multiworld rom/wad, ask for the player number. The text field is initially blank
                return Command::single(Action::Future(spawn_blocking(|| Python::with_gil(|py| {
                    let sys = py.import("sys")?;
                    sys.getattr("path")?.call_method1("append", (env!("CARGO_MANIFEST_DIR"),))?;
                    let settings = py.import("Settings")?.call_method1("Settings", (PyDict::new(py),))?; //TODO populate settings
                    let window = Window::new();
                    py.import("HintList")?.call_method0("clearHintExclusionCache")?;
                    let main = py.import("Main")?;
                    let rom = main.call_method1("resolve_settings", (settings, window))?;
                    let mut attempt = 0;
                    let spoiler = loop {
                        attempt += 1;
                        match main.call_method1("generate", (settings, window)) {
                            Ok(spoiler) => break spoiler,
                            Err(e) if e.is_instance(py, py.import("Fill")?.getattr("ShuffleError")?.downcast()?) => {
                                if attempt == 10 { return Err(e) }
                            }
                            Err(e) => return Err(e),
                        }
                        settings.call_method0("reset_distribution")?;
                    };
                    Ok((settings.to_object(py), window, spoiler.to_object(py), rom.to_object(py)))
                })).map(|res| match res {
                    Ok(Ok((settings, window, spoiler, rom))) => Message::SelectOutDir { settings, window, spoiler, rom },
                    Ok(Err(e)) => Message::Error(Arc::new(e.into())),
                    Err(e) => Message::Error(Arc::new(e.into())),
                }).boxed()))
            }
            Message::SelectOutDir { settings, window, spoiler, rom } => return Command::single(Action::Future(async move {
                if let Some(folder) = AsyncFileDialog::new().pick_folder().await {
                    spawn_blocking(move || Python::with_gil(move |py| {
                        settings.setattr(py, "output_dir", folder.path())?;
                        py.import("Main")?.call_method1("patch_and_output", (settings, window, spoiler, rom))?;
                        PyResult::Ok(())
                    })).await??
                }
                Ok(())
            }.map(|res| match res {
                Ok(()) => Message::Done,
                Err(e) => Message::Error(Arc::new(e)),
            }).boxed())),
        }
        Command::none()
    }

    fn view(&self) -> Element<'_, Message> {
        // see https://gist.github.com/fenhl/394e09e8ea5ac5e552c8c61d016992a6

        if let Some(ref e) = self.error {
            Text::new(e.to_string()).into()
        } else {
            Row::new()
            //TODO “Generate from” (dropdown, random seed/set seed/patch file)
            //TODO “Seed” (text field, only if “Generate from set seed”)
            //TODO “Base rom” (file select)
            //TODO “Settings” (dropdown with “Customize” button, only if “Generate from set/random seed”)
            //TODO “Cosmetics” (dropdown with “Customize” button)
            //TODO “Output type” (dropdown with options depending on world count)
            .push({
                let mut btn = Button::new(Text::new("Generate!"));
                if !self.generating { btn = btn.on_press(Message::Generate) }
                btn
            }) //TODO keep button enabled but style as disabled if prerequisites to generate seed not met (e.g. generate from patch file but no patch file)
            .into()
        }
    }
}

fn main() -> Result<(), Error> {
    Python::with_gil(|py| {
        let py_version = py.version_info();
        if py_version < (3, 6) {
            panic!("Randomizer requires at least Python 3.6 and you are using {}.{}.{}", py_version.major, py_version.minor, py_version.patch); //TODO GUI dialog
        }
    });
    Gui::run(iced::Settings::default())?;
    Ok(())
}
