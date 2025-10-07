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
    bool setupGPIOLines();
    bool setupTemperatureSensor();
    bool setClockDivisor();
    void setVoltage(double voltage);
    void setCurrent(double current);
    double readVoltage();
    double readCurrent();
    double readTemperature();
    void testread();



    //Global Data Variables
    //FTDI D2XX
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    DWORD bytesRead;
    bool m_isDeviceOpen = false;
    bool m_isMpsseOn = false;
    
    // Hardware state variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;
    bool hvOn = false;


    //UDP SOCKET
    int udpSocket;
    int udpSocketPort = 1030;
    int socketBufferSize = 1000;
    char *buffer = nullptr;


};




/*

D2XX USB HANDLER
FTDI CHİPSETLERİ İÇİN
MİNİXML KÜTÜPHANESİ KULLANILACAK

*/