#include "App.hpp"
using namespace std;

bool Controller::systemInitializor() {
        Debug.debugLevel = 3; // Set debug level (0-3)
        bool CurrentStatus = true;
        Debug.Log("==================================");
        Debug.Log("System Initializing..");
        Debug.Log("C++ Version: C++17");

        //Initialize Data Handler
        Debug.Log("Initializing Data Handler...");
        if(dataHandler.dataHandlerInit()){Debug.Log("Data Handler Initialized Successfully.");} 
        else {Debug.Error("Data Handler Initialization Failed!"); CurrentStatus = false;}


        if (CurrentStatus) isRunning = true;
        else{Debug.Error("System Initialization Failed!",5); isRunning = false;}
        return CurrentStatus;
    }

// PYTHON INTERFACE METHODS
#pragma region: PYTHON METHODS ----

    void Controller::connectMiniX(){dataHandler.connectMiniX();}

    void Controller::disconnectMiniX(){dataHandler.disconnectMiniX();}

    void Controller::testButton(){
        Debug.Log("=== STARTING DEVICE DIAGNOSTICS ===");
        pybind11::gil_scoped_release release;
        Debug.Log("Set Voltage = " + std::to_string(m_targetMinixVoltage.load()) + " kV");
        Debug.Log("Set Current = " + std::to_string(m_targetMinixCurrent.load()) + " uA");
    }

#pragma endregion: PYTHON METHODS ----

#pragma region: THREAD METHODS ----

void Controller::run(){
    pybind11::gil_scoped_release release;
        //Create and start the logic thread
        LogicThread = thread([this]() {
        while (isRunning) {
            logic();
            std::this_thread::sleep_for(10ms);
        }
        Debug.Log("App Stopped.");
        });
    }


void Controller::stop() {
        isRunning = false;
        if (LogicThread.joinable()) LogicThread.join();
        //End logic here
        dataHandler.disconnectMiniX();
    }

void Controller::logic(){
            elapsedMS = TimeM.elapsedMS();
            dataHandler.deviceStatusChecks(elapsedMS);
    }

#pragma endregion: THREAD METHODS ----