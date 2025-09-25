#include <pybind11/pybind11.h>
#include "../src/App.hpp"
namespace py = pybind11;

PYBIND11_MODULE(YDYTU1_py, m) {
    py::class_<App>(m, "App")
        .def(py::init<>())
        .def("Run", &App::Run)
        .def("Stop", &App::Stop);
}