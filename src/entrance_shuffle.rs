use {
    std::{
        collections::hash_map::DefaultHasher,
        hash::{
            Hash as _,
            Hasher as _,
        },
    },
    pyo3::{
        prelude::*,
        pyclass::CompareOp,
        types::PyBool,
    },
};

#[pyclass]
#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub(crate) enum EntranceKind {
    Dungeon,
    DungeonSpecial,
    ChildBoss,
    AdultBoss,
    SpecialBoss,
    Interior,
    SpecialInterior,
    Hideout,
    Grotto,
    Grave,
    Overworld,
    OverworldOneWay,
    OwlDrop,
    ChildSpawn,
    AdultSpawn,
    WarpSong,
    BlueWarp,
    Extra,
}

#[pymethods]
impl EntranceKind {
    fn __hash__(&self) -> isize {
        let mut hasher = DefaultHasher::new();
        self.hash(&mut hasher);
        hasher.finish() as _
    }
}

pub(crate) fn module(py: Python<'_>) -> PyResult<&PyModule> {
    let m = PyModule::new(py, "entrance_shuffle")?;
    m.add_class::<EntranceKind>()?;
    Ok(m)
}
