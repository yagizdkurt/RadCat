#include "ftd2xx.h"
#include "DataIOHandler.hpp"
#include "App.hpp" 
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"
#include "devices/comps/FTDIConnection.hpp"
#include <algorithm>
#include <cctype>

using namespace Utilities;

bool DIOHandler::dataHandlerInit(){

    return true;
}


// Minix Interface Methods
void DIOHandler::connectMiniX() {
    if(minixController.connectMiniX()){
        Debug.Log("Connected to MiniX successfully.");
    } else {
        Debug.Error("Failed to connect to MiniX.");
    }
}

void DIOHandler::disconnectMiniX() {
    if(!controller->m_connectedToMinix) return;
    else if(minixController.disconnectMiniX()){
        Debug.Log("Disconnected from MiniX successfully.");
    } else {
        Debug.Error("Failed to disconnect from MiniX.");
    }
}

void DIOHandler::minixAutoLogic(float elapsedMS){
    minixSafetyCheckPassed += elapsedMS;
    if(minixSafetyCheckPassed >= 250.0){
        minixSafetyCheckPassed = 0.0;
        if(!minixController.safetyChecks()) return;
    }

    tempReadPassed += elapsedMS;
    if(tempReadPassed >= 500.0){
        tempReadPassed = 0.0;
        // Read temperature
        controller ->m_latestMinixTemperature = minixController.readTemperature();
        // Debug.Log("Temperature: " + std::to_string(controller->m_latestMinixTemperature) + " C");
        controller ->m_newDataAvailable = true;
    }
}


// IO Auto Logic
void DIOHandler::deviceStatusChecks(float elapsedMS){
    if(controller->m_connectedToMinix) minixAutoLogic(elapsedMS);
}