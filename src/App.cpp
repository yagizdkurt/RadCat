#include "App.hpp"
using namespace std;

    void Controller::run(){
        //Create and start the logic thread
        LogicThread = thread([this]() {
        while (isRunning) {

            TimeM.timeUpdate();

            logic();

            this_thread::yield();
        }

        Debug.Log("App Stopped.");
        });
    }

    void Controller::stop() {
        isRunning = false;
        if (LogicThread.joinable()) LogicThread.join();
        //End logic here
    }

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

    void Controller::logic(){

        // Main logic loop
        if(m_connectedToMinix){
            //readVoltage();
            //readCurrent();
            //readTemperature();
            m_measurementCount++;
            m_newDataAvailable = true;
        }
        else{
            m_newDataAvailable = false;
        }

    }

    void Controller::testButton(){
        Debug.Log("=== STARTING DEVICE DIAGNOSTICS ===");
        pybind11::gil_scoped_release release;
        dataHandler.readTemperature();    // Then run the comprehensive test
    }





// PYTHON INTERFACE METHODS
#pragma region: PYTHON METHODS ----

    void Controller::connectMiniX(){dataHandler.connectMiniX();}

    void Controller::disconnectMiniX(){dataHandler.disconnectMiniX();}

#pragma endregion: PYTHON METHODS ----
