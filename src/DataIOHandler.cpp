#include "DataIOHandler.hpp"


bool DIOHandler::dataHandlerInit(){
    buffer = (char*)malloc(sizeof(char)*socketBufferSize);
    if(buffer == nullptr){Debug.Error("Failed to allocate memory for UDP buffer.");return false;}


    return true;
}


//INTERFACE METHODS
void DIOHandler::connectMiniX(){
    Debug.Log("Connecting to MiniX...");
    getConnectedDevices();
}

void DIOHandler::disconnectMiniX(){
    Debug.Log("Disconnecting from MiniX...");
    closeDevice();
}



// USB FTDI D2XX HANDLER METHODS
void DIOHandler::getConnectedDevices() {
    if (m_isDeviceOpen) {
        Debug.Log("Device is already open.");
        return;
    }

    FT_STATUS status;
    DWORD numDevs;

    // Get the number of connected devices
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status != FT_OK) { 
        Debug.Error("Error getting device list: " , status);
        return;
    }
    Debug.Log("Number of connected devices: " , numDevs);

    for (DWORD i = 0; i < numDevs; i++) {
        FT_DEVICE_LIST_INFO_NODE devInfo;
        status = FT_GetDeviceInfoDetail(i, &devInfo.Flags, &devInfo.Type, &devInfo.ID,
                                        &devInfo.LocId, devInfo.SerialNumber,
                                        devInfo.Description, &devInfo.ftHandle);
        if (status == FT_OK) {
            Debug.Log("Device:" + std::to_string(i) + " - " + devInfo.Description);
            Debug.Log("Serial Number:", devInfo.SerialNumber);
            Debug.Log("ID:", std::to_string(devInfo.ID));
            Debug.Log("Location ID:", std::to_string(devInfo.LocId));
        } else {
            Debug.Error("Error getting device info for device " + std::to_string(i) + ": " , status);
        }
    }
}