#pragma once
#include "ftd2xx.h"
#include "minixc.hpp"
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"

class DIOHandler;
class Controller;

//Serial number big= 1118880
class MinixController {
public:
    DIOHandler* DataHandler;
    Controller* controller;
    MinixController() : DataHandler(nullptr), controller(nullptr) {}
    ~MinixController() {}

    // Debugging
    static constexpr bool minixDebug = false;

    // Interface Methods
    bool connectMiniX();
    bool disconnectMiniX();
    double readVoltage();
    double readCurrent();
    double readTemperature();
    bool safetyChecks();

private:
    // Minix Setup and close Methods
    bool findMinixDevice();
    int getConnectedDevices();
    bool openDevice(const char* serialNumber);
    bool openMPSSE();
    bool initializeMiniX();
    bool closeDevice();
    bool setupTemperatureSensor();
    bool setupClockDivisor();
    void purgeMinixInfo();

    // Minix Checks
    bool connectionChecks();


    // Minix Control Methods
    void setVoltage(double voltage);
    void setCurrent(double current);
    void testread();

    //Global Data Variables
    //FTDI D2XX
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    DWORD bytesRead;

    // Mini-X Device Information Storage
    char minixSerialNumber[16];      // Store serial number (stable identifier)
    char minixDescription[64];       // Store device description
    DWORD minixLocationId;           // Store location ID (USB port)
    DWORD minixDeviceId;             // Store device/product ID
    bool isDeviceOpen = false;
    bool isMpsseOn = false;

    // Hardware state variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;
    bool hvOn = false;

};
