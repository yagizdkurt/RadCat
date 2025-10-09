#pragma once
#include "ftd2xx.h"
#include <windows.h>

class Controller;

class DIOHandler {
    
public:
    Controller* controller;
    DIOHandler(Controller* ctrl) : controller(ctrl) {}
    ~DIOHandler() {}

    bool dataHandlerInit();
    void connectMiniX();
    void disconnectMiniX();

    bool openDevice(const char* serialNumber);
    void closeDevice();
    bool writeBytes(const unsigned char* data, DWORD length);
    bool readBytes(unsigned char* buffer, DWORD length);
    void tryConnect();
    int getConnectedDevices();
    
    // MiniX initialization and control methods
    bool initializeMiniX();
    bool openMPSSE();
    bool setupTemperatureSensor();
    bool setupClockDivisor();
    void setVoltage(double voltage);
    void setCurrent(double current);
    double readVoltage();
    double readCurrent();
    double readTemperature();
    void testread();
    void debugConnectionStatus();
    bool findMinixDevice();

    //Global Data Variables
    //FTDI D2XX
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    DWORD bytesRead;

    // Mini-X Device Information Storage
    char m_minixSerialNumber[16];      // Store serial number (stable identifier)
    char m_minixDescription[64];       // Store device description
    DWORD m_minixLocationId;           // Store location ID (USB port)
    DWORD m_minixDeviceId;             // Store device/product ID
    bool m_minixDeviceFound = false;   // Track if Mini-X was found
    bool m_isDeviceOpen = false;
    bool m_isMpsseOn = false;
    
    // Hardware state variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;
    bool hvOn = false;

};
