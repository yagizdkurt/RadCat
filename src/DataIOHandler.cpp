#include "ftd2xx.h"
#include "DataIOHandler.hpp"
#include "App.hpp" 
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"

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
    if(minixController.disconnectMiniX()){
        Debug.Log("Disconnected from MiniX successfully.");
    } else {
        Debug.Error("Failed to disconnect from MiniX.");
    }
}

void DIOHandler::minixAutoLogic(float elapsedMS){
    tempReadPassed += elapsedMS;
    if(tempReadPassed >= tempReadMS){
        tempReadPassed = 0.0;
        // Read temperature
        controller ->m_latestTemperature = minixController.readTemperature();
        // Debug.Log("Temperature: " + std::to_string(controller->m_latestTemperature) + " C");
        controller ->m_newDataAvailable = true;
    }
}


// IO Auto Logic
void DIOHandler::deviceStatusChecks(float elapsedMS){
    if(controller->m_connectedToMinix) minixAutoLogic(elapsedMS);
}