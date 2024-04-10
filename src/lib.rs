use pyo3::prelude::*;

macro_rules! py_mod {
    ($($name:ident,)*) => {
        $(mod $name;)*

        #[pymodule]
        fn rs(py: Python<'_>, m: &PyModule) -> PyResult<()> {
            let sys_modules = py.import("sys")?.getattr("modules")?;
            $(
                let $name = $name::module(py)?;
                m.add_submodule($name)?;
                sys_modules.set_item(concat!("rs.", stringify!($name)), $name)?;
            )*
            Ok(())
        }
    };
}

py_mod!(
    entrance_shuffle,
    hints,
    version,
);
