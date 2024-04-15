use {
    std::num::NonZeroU8,
    anyhow::Context as _,
    lazy_regex::regex_captures,
    pyo3::prelude::*,
    semver::Version,
};

pub(crate) fn module(py: Python<'_>) -> PyResult<Bound<'_, PyModule>> {
    let Version { major, minor, patch, pre, build: _ } = env!("CARGO_PKG_VERSION").parse().context("failed to parse cargo package version")?;
    let (_, fenhl, riir) = regex_captures!(r"^fenhl\.([0-9]+)\.riir\.([0-9]+)$", &pre).context("failed to parse cargo package version suffix")?;
    let riir = riir.parse::<NonZeroU8>()?;
    let m = PyModule::new_bound(py, "version")?;
    m.add("supplementary_version", riir.get())?;
    m.add("branch_identifier", 0xff_u8)?;
    m.add("branch_url", "https://github.com/fenhl/OoT-Randomizer/tree/riir1")?;
    m.add("base_version", format!("{major}.{minor}.{patch}"))?;
    m.add("__version__", format!("{major}.{minor}.{patch} Fenhl-{fenhl} riir-{riir}"))?;
    Ok(m)
}
