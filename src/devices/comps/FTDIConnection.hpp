#pragma once
#include <ftd2xx.h>
#include <string>
#include "componentCore.hpp"

COMPONENT class FTDIConnection : public BaseComponent {
public:
    template<typename DeviceType> FTDIConnection(DeviceType& parentDevice) : BaseComponent(&parentDevice) {
        // Initialization
        Initialize();
    }

    //Interface Methods
    FT_STATUS sendData(const unsigned char* data, DWORD size);
    FT_STATUS receiveData(unsigned char* buffer, DWORD size, DWORD& bytesRead);
    bool PollData(DWORD bytesToRead, DWORD& bytesRead, int timeoutMs);
    bool fConnect();
    bool fDisconnect();

    //Getters and Setters
    void setFTDIIndex(int index) { if (FTDIIndex != -1) return; FTDIIndex = index; } //Set only if not set
    void setDevInfo(const FT_DEVICE_LIST_INFO_NODE& info) { devInfo = info; }
    int getFTDIIndex() const { return FTDIIndex; }
    FT_DEVICE_LIST_INFO_NODE getDevInfo() const { return devInfo; }
    bool isConnected() const { return connected; }
    bool isDeviceOpen() const { return deviceIsOpen; }
    bool isMPSSEOn() const { return setupDone; }
    bool isTryingToConnect() const { return tryingToConnect; }

private:
    
    static constexpr bool debug = false; //Debug flag
    FT_HANDLE ftHandle = nullptr;
    FT_STATUS ftStatus = FT_OK;
    FT_DEVICE_LIST_INFO_NODE devInfo;
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;
    std::string myDeviceName = "";
    int FTDIIndex = -1;
    bool setupDone = false;
    bool connected = false;
    bool tryingToConnect = false;
    bool deviceIsOpen = false;
    bool openMPSSE();
    bool openDevice();
    void purgeBuffers();
    bool closeDevice();
    bool Initialize();

};