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
            
            // Continue with normal logic
            dataHandler.deviceStatusChecks(elapsedMS);

            // Process any queued tasks from Python
            processQueuedTasks();
        }

#pragma endregion: THREAD METHODS ----

#pragma region: TASK QUEUE METHODS ----

void Controller::queueTask(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(m_taskMutex);
    m_taskQueue.push(task);
}

void Controller::processQueuedTasks() {
    std::unique_lock<std::mutex> lock(m_taskMutex);

    while (!m_taskQueue.empty()) {
        auto task = m_taskQueue.front();
        m_taskQueue.pop();
        
        // Temporarily unlock mutex while executing task
        lock.unlock();
        try {
            task();  // Execute the task
        } catch (const std::exception& e) {
            Debug.Error("Task execution failed: " + std::string(e.what()));
        }
        lock.lock();  // Re-lock for next iteration
    }
}

#pragma endregion: TASK QUEUE METHODS ----