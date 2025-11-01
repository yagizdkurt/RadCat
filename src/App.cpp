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

        Debug.Log("Initializing UDP Handler...");
        if(udpHandler.start()){Debug.Log("UDP Handler Initialized Successfully.");} 
        else {Debug.Error("UDP Handler Initialization Failed!"); CurrentStatus = false;}

        if (CurrentStatus){Debug.Log("All systems go!"); isRunning = true;}
        else{Debug.Error("System Initialization Failed!",5); isRunning = false;}
        return CurrentStatus;
}

#pragma region: THREAD METHODS ----

void Controller::run(){
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
        udpHandler.stop();
    }

void Controller::logic(){
            elapsedMS = TimeM.elapsedMS(); // Measure elapsed time since last call
            dataHandler.deviceStatusChecks(elapsedMS); // Device Auto Logic
            processQueuedTasks(); // Process any queued tasks from Python

            // UDP Message Processing
            udpHandler.processIncomingData(); // Read all available data from socket into queue
            while (udpHandler.hasQueuedMessages()) {
                std::string data = udpHandler.receiveData();
                if (!data.empty()) {
                    Debug.Log("Processing queued UDP message: " + data + 
                             " (Queue size: " + std::to_string(udpHandler.getQueueSize()) + ")");
                    processUDPData(data);
                }
            }
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


void Controller::processUDPData(string data) {
    queueTask([this, data]() {
        Debug.Log("Processing UDP command: '" + data + "'");
        
        if (data == "GET_STATUS") {
            Debug.Log("UDP Command: Get Status");
            // TODO: Send status response back to client
        } else if (data == "START_SCAN") {
            Debug.Log("UDP Command: Start Scan");
            // TODO: Start scanning operation
        } else if (data == "STOP_SCAN") {
            Debug.Log("UDP Command: Stop Scan");
            // TODO: Stop scanning operation
        } else if (data.substr(0, 12) == "SET_VOLTAGE:") {
            std::string voltage_str = data.substr(12);
            Debug.Log("UDP Command: Set Voltage to " + voltage_str + " kV");
            dataHandler.setVoltage(std::stod(voltage_str));
        } else if (data.substr(0, 12) == "SET_CURRENT:") {
            std::string current_str = data.substr(12);
            Debug.Log("UDP Command: Set Current to " + current_str + " uA");
            dataHandler.setCurrent(std::stod(current_str));
        } else {
            Debug.Log("UDP Command: Unknown command '" + data + "'");
        }
    });
}

