#![deny(warnings)]

use pyo3::prelude::*;

macro_rules! py_mod {
    ($($name:ident,)*) => {
        $(mod $name;)*

        #[pymodule]
        fn rs(py: Python<'_>, m: Bound<'_, PyModule>) -> PyResult<()> {
            let sys_modules = py.import_bound("sys")?.getattr("modules")?;
            $(
                let $name = $name::module(py)?;
                m.add_submodule(&$name)?;
                sys_modules.set_item(concat!("rs.", stringify!($name)), $name)?;
            )*
            Ok(())
        }
    };
}

py_mod!(
    entrance_shuffle,
    hints,
    search,
    version,
);
