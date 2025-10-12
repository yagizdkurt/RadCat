#pragma once
#include <pybind11/pybind11.h>

class Controller; // Forward declaration
class PyManager {
public:
    Controller* controller;

    PyManager(Controller* ctrl) : controller(ctrl) {}
    ~PyManager() {}

    // PYTHON INTERFACE METHODS
    void connectMiniX();
    void disconnectMiniX();
    void testButton();
    void setTargetVoltage(double voltage);
    void setTargetCurrent(double voltage);

};