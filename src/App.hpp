#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <pybind11/pybind11.h>
#include "DataIOHandler.hpp"
#include "TimeManager.hpp"
#include "Debug.hpp"

using namespace std;
class Controller{
private:
    mutex dataMutex;
    thread LogicThread;
    unique_ptr<DIOHandler> dataHandler_ptr;

public:
    //Modules
    DIOHandler& dataHandler;

    Controller() : dataHandler_ptr(std::make_unique<DIOHandler>(this)), dataHandler(*dataHandler_ptr)
    {
        if(systemInitializor()) run();
    }

    ~Controller(){}

    //Main Methods
    void run();
    void logic();
    void stop();
    bool systemInitializor();

    //Data Call Methods
    void readVoltage() { m_latestVoltage = dataHandler.readVoltage(); }
    void readCurrent() { m_latestCurrent = dataHandler.readCurrent(); }
    void readTemperature() { m_latestTemperature = dataHandler.readTemperature(); }


    // PYTHON INTERFACE METHODS
    void connectMiniX();
    void disconnectMiniX();
    void testButton();

    // ----- SHARED DATA -----
    // Checking alot
    atomic<double> m_latestVoltage{0.0};
    atomic<double> m_latestCurrent{0.0};
    atomic<double> m_latestTemperature{0.0};
    atomic<uint64_t> m_lastUpdateTimestamp{0};
    atomic<bool> m_newDataAvailable{false};
    atomic<bool> m_isMpsseOn{false};
    atomic<bool> hvOn{false};
    atomic<int> m_errorCount{0};
    atomic<int> m_measurementCount{0};

    // Setup and config data
    atomic<bool> isRunning{false};
    atomic<bool> m_minixDeviceFound{false};
    atomic<bool> m_tryingToConnectMinix{false};
    atomic<bool> m_minixOpened{false};
    atomic<bool> m_connectedToMinix{false};

};