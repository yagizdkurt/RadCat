#pragma once
#include <ftd2xx.h>
#include <string>
#include "componentCore.hpp"

template<typename... Components> class BaseDevice;
class EmptyDevice;

class FTDIConnection : public BaseComponent {
public:
    template<typename DeviceType> FTDIConnection(DeviceType& parentDevice) : 
    parent(&parentDevice),
    ftHandle(nullptr),
    ftStatus(FT_OK),
    bytesWritten(0),
    bytesRead(0),
    FTDIIndex(-1),
    tryingToConnect(false),
    connected(false),
    deviceIsOpen(false),
    setupDone(false),
    myDeviceName("") {
        // Initialization
        Initialize();
    }

    //Interface Methods
    FT_STATUS sendData(const unsigned char* data, DWORD size);
    FT_STATUS receiveData(unsigned char* buffer, DWORD size, DWORD& bytesRead);
    bool connectToDevice();
    void disconnectFromDevice();

    //Getters and Setters
    void setFTDIIndex(int index) { if (FTDIIndex != -1) return; FTDIIndex = index; } //Set only if not set
    void setDevInfo(const FT_DEVICE_LIST_INFO_NODE& info) { devInfo = info; }
    int getFTDIIndex() const { return FTDIIndex; }
    FT_DEVICE_LIST_INFO_NODE getDevInfo() const { return devInfo; }
    bool isConnected() const { return connected; }

    bool tryingToConnect = false;
    bool deviceIsOpen = false;

private:
    
    static constexpr bool debug = false; //Debug flag
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    FT_DEVICE_LIST_INFO_NODE devInfo;
    DWORD bytesWritten;
    DWORD bytesRead;
    std::string myDeviceName;
    int FTDIIndex = -1;
    bool setupDone = false;
    bool connected = false;
    EmptyDevice* parent;
    bool openMPSSE();
    bool openDevice();
    void purgeBuffers();
    void closeDevice();
    bool Initialize();

};