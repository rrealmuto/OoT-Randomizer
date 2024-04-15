use pyo3::{
    prelude::*,
    types::{
        IntoPyDict as _,
        PyDict,
        PySet,
    },
};

#[pyclass(get_all, set_all)]
struct SearchCache {
    child_queue: Vec<Py<PyAny>>,
    adult_queue: Vec<Py<PyAny>>,
    visited_locations: Py<PySet>,
    child_regions: Py<PyDict>,
    adult_regions: Py<PyDict>,
}

#[pymethods]
impl SearchCache {
    #[new]
    #[pyo3(signature = (*, child_queue, adult_queue, visited_locations, child_regions, adult_regions))]
    fn new(
        child_queue: Vec<Py<PyAny>>,
        adult_queue: Vec<Py<PyAny>>,
        visited_locations: Py<PySet>,
        child_regions: Py<PyDict>,
        adult_regions: Py<PyDict>,
    ) -> Self {
        Self { child_queue, adult_queue, visited_locations, child_regions, adult_regions }
    }

    fn copy(&self, py: Python<'_>) -> PyResult<Self> {
        Ok(Self {
            child_queue: self.child_queue.iter().map(|elt| elt.clone_ref(py)).collect(),
            adult_queue: self.adult_queue.iter().map(|elt| elt.clone_ref(py)).collect(),
            visited_locations: PySet::new_bound(py, PySetMethods::iter(self.visited_locations.bind(py)).map(|elt| elt.into_gil_ref()))?.unbind(),
            child_regions: PyDictMethods::iter(self.child_regions.bind(py)).into_py_dict_bound(py).unbind(),
            adult_regions: PyDictMethods::iter(self.adult_regions.bind(py)).into_py_dict_bound(py).unbind(),
        })
    }
}

pub(crate) fn module(py: Python<'_>) -> PyResult<Bound<'_, PyModule>> {
    let m = PyModule::new_bound(py, "search")?;
    m.add_class::<SearchCache>()?;
    Ok(m)
}
