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
        Debug.Log("System Initializing...");

        // Perform initialization tasks here


        if (CurrentStatus) isRunning = true;
        else{
            Debug.Error("System Initialization Failed!",5);
            isRunning = false;
        }
        return CurrentStatus;
    }

    void App::logic(){

    }