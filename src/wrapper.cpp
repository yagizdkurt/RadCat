#include <pybind11/pybind11.h>
#include "../src/App.hpp"
namespace py = pybind11;

PYBIND11_MODULE(YDYTU1_py, m) {
    py::class_<Controller>(m, "Controller")

        // Functions
        .def(py::init<>())
        .def("run", &Controller::run)
        .def("stop", &Controller::stop)
        .def("connectMiniX", &Controller::connectMiniX)
        .def("testButton", &Controller::testButton)
        .def("disconnectMiniX", &Controller::disconnectMiniX)

        // Read-only properties for Setup and config data
        .def_property_readonly("minixDeviceFound", [](const Controller& c) { return c.m_minixDeviceFound.load(); })
        .def_property_readonly("tryingToConnectMinix", [](const Controller& c) { return c.m_tryingToConnectMinix.load(); })
        .def_property_readonly("minixOpened", [](const Controller& c) { return c.m_minixOpened.load(); })
        .def_property_readonly("isMpsseOn", [](const Controller& c) { return c.m_isMpsseOn.load(); })
        .def_property_readonly("connectedToMinix", [](const Controller& c) { return c.m_connectedToMinix.load(); })

        // Read-only properties for atomic variables
        .def_property_readonly("isRunning", [](const Controller& c) { return c.isRunning.load(); })
        .def_property_readonly("latestVoltage", [](const Controller& c) { return c.m_latestVoltage.load(); })
        .def_property_readonly("latestCurrent", [](const Controller& c) { return c.m_latestCurrent.load(); })
        .def_property_readonly("latestTemperature", [](const Controller& c) { return c.m_latestTemperature.load(); })
        .def_property_readonly("newDataAvailable", [](const Controller& c) { return c.m_newDataAvailable.load(); })
        .def_property_readonly("measurementCount", [](const Controller& c) { return c.m_measurementCount.load(); });
}