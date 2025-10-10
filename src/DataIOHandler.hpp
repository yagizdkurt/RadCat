#pragma once
#include "ftd2xx.h"
#include "minixc.hpp"

class Controller;

class DIOHandler {
    
public:
    Controller* controller;
    MinixController minixController;

    DIOHandler(Controller* ctrl) : controller(ctrl) {
        minixController.controller = ctrl;
        minixController.DataHandler = this;
    }

    ~DIOHandler() {}

    bool dataHandlerInit();

    //Minix Interface Methods
    void connectMiniX();
    void disconnectMiniX();

    //IO Auto Logic
    void deviceStatusChecks(float elapsedMS);
    void minixAutoLogic(float elapsedMS);
    float tempReadMS = 500.0; float tempReadPassed = 0.0;


};
