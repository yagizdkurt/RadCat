#include "FTDIConnection.hpp"
#include "debug.hpp"
#include "Utilities.hpp"
#include "deviceCore.hpp"

using namespace Utilities;

bool FTDIConnection::fConnect() {
    if (connected) return true;
    tryingToConnect = true;

    if(!openDevice()) { tryingToConnect = false; connected = false; return false; }
    if(!openMPSSE()) { tryingToConnect = false; connected = false; return false; }

    connected = true;
    tryingToConnect = false;
    return true; // Successfully connected
}

bool FTDIConnection::fDisconnect() {
    if (!connected && !deviceIsOpen) return true;
    if(!closeDevice()) return false;
    connected = false;
    deviceIsOpen = false;
    if constexpr (debug) Debug.Log("FTDI device disconnected successfully.");
    return true;
}

bool FTDIConnection::closeDevice(){
    if (!deviceIsOpen) return true;
    purgeBuffers();
    FT_ResetDevice(ftHandle);
    FT_STATUS status = FT_Close(ftHandle);
    if (status != FT_OK) { Debug.Error("Failed to close FTDI device: " + std::to_string(status)); return false; }
    deviceIsOpen = false;
    ftHandle = nullptr;
    if constexpr (debug) Debug.Log("FTDI device closed successfully.");
    return true;
}

bool FTDIConnection::openDevice(){
    if (deviceIsOpen) return true;

    ftStatus = FT_Open(FTDIIndex, &ftHandle);
    if (ftStatus != FT_OK) {
        Debug.Error("Failed to open FTDI device: "+ std::to_string(FTDIIndex) + ", " + std::to_string(ftStatus));
        tryingToConnect = false;
        connected = false;
        return false;
    }

    deviceIsOpen = true;
    if constexpr (debug) Debug.Log("FTDI device opened successfully.");
    FT_ResetDevice(ftHandle); // Reset device to ensure clean state
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX); // Clear RX and TX buffers
    sleepMs(100); // Wait for device to stabilize
    return true;
}

bool FTDIConnection::openMPSSE() {
    switch (devInfo.Type) { // Devices that dont need MPSSE
        case FT_DEVICE_2232C:
        case FT_DEVICE_232R:
        case FT_DEVICE_AM:
        case FT_DEVICE_BM:
        case FT_DEVICE_100AX:
            return true;
    }

    unsigned char tx[5]; unsigned char rx[5]; FT_STATUS status; DWORD ret_bytes;
    //Set USB Parameters and Latency Timer
    FT_SetUSBParameters(ftHandle, 65536, 65536); //Set USB request transfer sizes
    FT_SetLatencyTimer(ftHandle, 4); //4ms
    FT_SetTimeouts(ftHandle, 40, 40); //40ms read/write timeouts
    FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0); 
    sleepMs(20);

    status = FT_SetBitMode(ftHandle, 0x0, 0x02);  //enable MPSSE 
    if(status != FT_OK){printf("Error enabling serial engine (MPSSE). Check controller connection.\n");return false;}
    sleepMs(20); purgeBuffers();

    //Test MPSSE by sending command 0xAA and expecting response 0xFA 0xAA
    tx[0] = 0xAA;
    FT_Write(ftHandle, tx, 1, &ret_bytes);
    sleepMs(20);
    FT_Read(ftHandle, rx, 2, &ret_bytes);
    // Expect: 0xFA 0xAA back
    if (ret_bytes == 2 && rx[0] == 0xFA && rx[1] == 0xAA) { if constexpr (debug) Debug.Log("MPSSE ENGINE OK."); }
    else { Debug.Error("Unexpected response from MPSSE engine."); return false; }
    
    return true;
}

void FTDIConnection::purgeBuffers(){ FT_STATUS status = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX); sleepMs(20); }

FT_STATUS FTDIConnection::sendData(const unsigned char* data, DWORD size){
    if (!data) { Debug.Error("FTDI sendData: null data pointer"); return FT_INVALID_PARAMETER; }
    if (size == 0) { Debug.Warn("FTDI sendData: zero size requested"); return FT_OK; }
    if (!ftHandle) { Debug.Error("FTDI sendData: device not connected"); return FT_INVALID_HANDLE; }
    DWORD bytesWritten = 0;
    ftStatus = FT_Write(ftHandle, (LPVOID)data, size, &bytesWritten);
    if(ftStatus != FT_OK){ Debug.Error("FTDI Write Error: " + std::to_string(ftStatus)); }
    else if(bytesWritten != size) {Debug.Warn("FTDI sendData: requested " + std::to_string(size) + " bytes, but wrote " + std::to_string(bytesWritten) + " bytes.");}
    return ftStatus;
}

FT_STATUS FTDIConnection::receiveData(unsigned char* buffer, DWORD size, DWORD& bytesRead){
    bytesRead = 0;
    if (!buffer) { Debug.Error("FTDI receiveData: null buffer pointer"); return FT_INVALID_PARAMETER; }
    if (size == 0) { Debug.Warn("FTDI receiveData: zero size requested"); return FT_OK; }
    if (!ftHandle) { Debug.Error("FTDI receiveData: device not connected"); return FT_INVALID_HANDLE; }
    ftStatus = FT_Read(ftHandle, buffer, size, &bytesRead);
    if (ftStatus != FT_OK) { Debug.Error("FTDI Read Error: " + std::to_string(ftStatus)); }
    else if (bytesRead == 0) { Debug.Warn("FTDI Read: no data available"); }
    else if constexpr (debug) { Debug.Log("FTDI received " + std::to_string(bytesRead) + " bytes"); }
    return ftStatus;
}

bool FTDIConnection::Initialize() {
    memset(&devInfo, 0, sizeof(devInfo));
    myDeviceName = parent->deviceInfo.deviceName;
    return true;
}

bool FTDIConnection::PollData(DWORD bytesToRead, DWORD& bytesRead, int timeoutMs) {
    if(ftHandle == nullptr || bytesToRead == 0) {Debug.Error("PollData: Invalid handle or bytesToRead."); return false;}
    if(timeoutMs <= 0) {Debug.Error("PollData: timeout must be positive."); return false;}
    DWORD rxBytes = 0; bytesRead = 0; int elapsed = 0; const int pollInterval = 20;
    
    while (bytesRead < bytesToRead && elapsed < timeoutMs) {
        FT_GetQueueStatus(ftHandle, &rxBytes);
        if (rxBytes > 0) {
            bytesRead = rxBytes;
            if (bytesRead > bytesToRead) { bytesRead = bytesToRead; }
        }
        Utilities::sleepMs(pollInterval);
        elapsed += pollInterval;
    }
    if (bytesRead < bytesToRead) { Debug.Warn("PollData: Timeout waiting for data. Requested: " + std::to_string(bytesToRead) +  ", Received: " + std::to_string(bytesRead)); return false; } 
    else { 
        Debug.Log("PollData: Successfully polled " + std::to_string(bytesRead) + " bytes of data."); 
        return true; 
    }
}