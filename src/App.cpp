#include "App.hpp"
using namespace std;

    void App::run(){
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

    void App::stop() {
        isRunning = false;
        if (LogicThread.joinable()) LogicThread.join();
        //End logic here

    }

    bool App::systemInitializor() {
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

    void App::logic(){

    }





// PYTHON INTERFACE METHODS
#pragma region: PYTHON METHODS ----

    void App::connectMiniX(){dataHandler.connectMiniX();}

    void App::disconnectMiniX(){dataHandler.disconnectMiniX();}

#pragma endregion: PYTHON METHODS ----
