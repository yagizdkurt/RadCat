#include <pybind11/pybind11.h>
#include "../src/App.hpp"
namespace py = pybind11;

PYBIND11_MODULE(YDYTU1_py, m) {
    py::class_<Controller>(m, "Controller")
        .def(py::init<>())
        .def("run", &Controller::run)
        .def("stop", &Controller::stop)
        .def("connectMiniX", &Controller::connectMiniX)
        .def("disconnectMiniX", &Controller::disconnectMiniX);
}