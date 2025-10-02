#pragma once
#include "includes.hpp"
#include "ftd2xx.h"

class DIOHandler {
    
public:
    DIOHandler() {}
    ~DIOHandler() {}

    bool dataHandlerInit();
    void connectMiniX();
    void disconnectMiniX();



    bool openDevice(int deviceIndex);
    void closeDevice();
    bool writeBytes(const unsigned char* data, DWORD length);
    bool readBytes(unsigned char* buffer, DWORD length);
    void tryConnect();
    void getConnectedDevices();



    //Global Data Variables
    //FTDI D2XX
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    DWORD bytesRead;
    bool m_isDeviceOpen = false;


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