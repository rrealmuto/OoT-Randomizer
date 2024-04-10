#![allow(warnings)] //TODO

use {
    std::{
        fs,
        path::{
            Path,
            PathBuf,
        },
    },
    pyo3::{
        prelude::*,
        types::PyDict,
    },
};

#[derive(Default, Clone, Copy, clap::ValueEnum)]
enum LogLevel {
    Error,
    Warning,
    #[default]
    Info,
    Debug,
}

impl LogLevel {
    fn try_into_py<'p>(&self, py: Python<'p>) -> PyResult<&'p PyAny> {
        let logging = py.import("logging")?;
        match self {
            Self::Error => logging.getattr("ERROR"),
            Self::Warning => logging.getattr("WARNING"),
            Self::Info => logging.getattr("INFO"),
            Self::Debug => logging.getattr("DEBUG"),
        }
    }
}

#[derive(clap::Parser)]
#[clap(version)]
struct Args {
    /// Select level of logging for output.
    #[clap(long, alias = "loglevel", value_enum, default_value_t)]
    log_level: LogLevel,
    /// Provide sharable settings using a settings string. This will override all flags that it specifies.
    #[clap(long, alias = "settings_string")]
    settings_string: Option<String>,
    /// Only convert the specified settings to a settings string. If a settings string is specified output the used settings instead.
    #[clap(long, alias = "convert_settings")]
    convert_settings: bool,
    /// Use the specified settings file to use for generation
    #[clap(long)]
    settings: Option<PathBuf>,
    /// Use the given preset for base settings. Anything defined in the --settings file or the --settings_string will override the preset.
    #[clap(long, alias = "preset", alias = "settings_preset")]
    settings_preset: Option<String>,
    /// Generate the specified seed.
    #[clap(long)]
    seed: Option<String>,
    /// Suppresses the generation of a log file.
    #[clap(long, alias = "no_log")]
    no_log: bool,
    /// Always outputs a settings.json file even when spoiler is enabled.
    #[clap(long, alias = "output_settings")]
    output_settings: bool,
    /// Generates a ZPF patch from the specified ROM file.
    #[clap(long, alias = "diff_rom")]
    diff_rom: bool,
}

#[derive(Debug, thiserror::Error)]
enum Error {
    #[error(transparent)] Io(#[from] std::io::Error),
    #[error(transparent)] Python(#[from] PyErr),
}

impl<'a> From<pyo3::PyDowncastError<'a>> for Error {
    fn from(e: pyo3::PyDowncastError<'a>) -> Self {
        Self::Python(e.into())
    }
}

#[wheel::main]
fn main(Args { log_level, settings_string, convert_settings, settings, settings_preset, seed, no_log, output_settings, diff_rom }: Args) -> Result<i32, Error> {
    match Python::with_gil(|py| {
        let sys = py.import("sys")?;
        sys.getattr("path")?.call_method1("append", (env!("CARGO_MANIFEST_DIR"),))?;
        let json = py.import("json")?;
        let settings_mod = py.import("Settings")?;
        let utils = py.import("Utils")?;
        utils.call_method0("check_python_version")?;
        let settings_base = PyDict::new(py);
        if let Some(preset_name) = settings_preset {
            if let Some(preset) = settings_mod.call_method0("get_preset_files")?.iter()?.filter_map(|filename| filename.map_err(Error::from).and_then(|filename| {
                let presets = json.call_method1("loads", (fs::read_to_string(filename.extract::<&str>()?)?,))?.downcast::<PyDict>()?;
                presets.get_item(&preset_name).map_err(Error::from) //TODO deal with preset aliases
            }).transpose()).next() {
                settings_base.call_method1("update", (preset?,))?;
            } else {
                eprintln!("ERROR:No preset found with name {preset_name:?}");
                return Ok(1)
            }
        }
        if settings.as_ref().map_or(false, |settings| settings == Path::new("-")) {
            settings_base.call_method1("update", (json.call_method1("loads", (sys.getattr("stdin")?.call_method0("read")?,))?,))?;
        } else if settings.is_some() || settings_base.is_empty() { // avoid implicitly using settings.sav with presets
            let settings_path = utils.call_method1("local_path", (settings.as_deref().unwrap_or(Path::new("settings.sav")),))?.extract::<&str>()?;
            if let Err(e) = fs::read_to_string(settings_path).map_err(Error::from).and_then(|settings| {
                settings_base.call_method1("update", (json.call_method1("loads", (settings,))?,))?;
                Ok(())
            }) {
                if settings.is_some() { return Err(e) }
            }
        }
        let settings = settings_mod.call_method1("Settings", (settings_base,))?;
        settings.setattr("output_settings", output_settings)?;
        if let Some(ref settings_string) = settings_string {
            settings.call_method1("update_with_settings_string", (settings_string,))?;
        }
        if let Some(seed) = seed {
            settings.call_method1("update_seed", (seed,))?;
            settings.setattr("custom_seed", true)?;
        }
        if convert_settings {
            if settings_string.is_some() {
                println!("{}", json.call_method1("dumps", (settings.call_method0("to_json")?,))?);
            } else {
                println!("{}", settings.call_method0("get_settings_string")?);
            }
            return Ok(0)
        }
        py.import("OoTRandomizer")?.call_method1("start", (settings, log_level.try_into_py(py)?, no_log, diff_rom))?;
        Ok(0)
    }) {
        Err(Error::Python(e)) => {
            Python::with_gil(|py| e.print(py));
            Ok(1)
        }
        result => result,
    }
}
