#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <pybind11/pybind11.h>
#include "TimeManager.hpp"
#include "Debug.hpp"
#include "DataIOHandler.hpp"

using namespace std;

class Controller{
private:
    mutex dataMutex;
    thread LogicThread;
    float elapsedMS = 0.0f;

public:
    //Modules
    DIOHandler dataHandler;

    Controller() : dataHandler(this) { if(systemInitializor()) run(); }
    ~Controller(){}

    //Main Methods
    void run();
    void logic();
    void stop();
    bool systemInitializor();

    // PYTHON INTERFACE METHODS
    void connectMiniX();
    void disconnectMiniX();
    void testButton();

    // ----- SHARED DATA -----

    // Shared Minix Data
    atomic<double> m_latestMinixVoltage{0.0};
    atomic<double> m_latestMinixCurrent{0.0};
    atomic<double> m_latestMinixTemperature{0.0};
    atomic<double> m_targetMinixVoltage{0.0};
    atomic<double> m_targetMinixCurrent{0.0};
    atomic<bool> m_minixDeviceFound{false};
    atomic<bool> m_tryingToConnectMinix{false};
    atomic<bool> m_minixOpened{false};
    atomic<bool> m_connectedToMinix{false};




    atomic<uint64_t> m_lastUpdateTimestamp{0};
    atomic<bool> m_newDataAvailable{false};
    atomic<bool> m_isMpsseOn{false};
    atomic<bool> hvOn{false};
    atomic<int> m_errorCount{0};
    atomic<int> m_measurementCount{0};

    // Setup and config
    atomic<bool> isRunning{false};



};