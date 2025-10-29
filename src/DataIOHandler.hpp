#pragma once
#include "ftd2xx.h"
#include "minixc.hpp"

class Controller;

class DIOHandler {
    
public:
    Controller* controller;
    static constexpr bool debug = false;

    DIOHandler(Controller* ctrl) : controller(ctrl) {
        minixController.controller = ctrl;
        minixController.DataHandler = this;
    }

    ~DIOHandler() {}

    bool dataHandlerInit();

    //Minix Interface Methods
    void connectMiniX();
    void disconnectMiniX();
    void setVoltage(double voltage) { minixController.setVoltage(voltage); }
    void setCurrent(double current) { minixController.setCurrent(current); }

    //IO Auto Logic
    void deviceStatusChecks(float elapsedMS);
    float tempReadPassed = 0.0; float minixSafetyCheckPassed = 0.0;

private:
    MinixController minixController;
    void minixAutoLogic(float elapsedMS);

};
