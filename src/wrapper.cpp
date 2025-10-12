#include <pybind11/pybind11.h>
#include "../src/App.hpp"
#include "../src/PyManager.hpp"

namespace py = pybind11;

PYBIND11_MODULE(YDYTU1_py, m) {

    py::class_<PyManager>(m, "PyManager")
        .def("connectMiniX", &PyManager::connectMiniX)
        .def("disconnectMiniX", &PyManager::disconnectMiniX)
        .def("testButton", &PyManager::testButton)
        .def("setTargetVoltage", [](PyManager& c, double voltage) {c.setTargetVoltage(voltage);})
        .def("setTargetCurrent", [](PyManager& c, double current) {c.setTargetCurrent(current);})

        ;

    py::class_<Controller>(m, "Controller")

        // Functions
        .def(py::init<>())
        .def("getPyManager", &Controller::getPyManager, py::return_value_policy::reference_internal)
        .def("run", &Controller::run)
        .def("stop", &Controller::stop)

        // Read-only properties for Setup and config data
        .def_property_readonly("minixDeviceFound", [](const Controller& c) { return c.m_minixDeviceFound.load(); })
        .def_property_readonly("tryingToConnectMinix", [](const Controller& c) { return c.m_tryingToConnectMinix.load(); })
        .def_property_readonly("minixOpened", [](const Controller& c) { return c.m_minixOpened.load(); })
        .def_property_readonly("isMpsseOn", [](const Controller& c) { return c.m_isMpsseOn.load(); })
        .def_property_readonly("connectedToMinix", [](const Controller& c) { return c.m_connectedToMinix.load(); })

        // Read-only properties for atomic variables
        .def_property_readonly("isRunning", [](const Controller& c) { return c.isRunning.load(); })
        .def_property_readonly("latestVoltage", [](const Controller& c) { return c.m_latestMinixVoltage.load(); })
        .def_property_readonly("latestCurrent", [](const Controller& c) { return c.m_latestMinixCurrent.load(); })
        .def_property_readonly("latestTemperature", [](const Controller& c) { return c.m_latestMinixTemperature.load(); })
        .def_property_readonly("newDataAvailable", [](const Controller& c) { return c.m_newDataAvailable.load(); })
        .def_property_readonly("measurementCount", [](const Controller& c) { return c.m_measurementCount.load(); })
        
        ;

}