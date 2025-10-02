#include "DataIOHandler.hpp"

void DIOHandler::dataHandlerInit(){
    buffer = (char*)malloc(sizeof(char)*socketBufferSize);
}

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
            std::cout << "Device " << i << ": " << devInfo.Description
                      << ", Serial Number: " << devInfo.SerialNumber
                      << ", ID: " << std::hex << devInfo.ID << std::dec
                      << ", Location ID: " << devInfo.LocId << std::endl;
        } else {
            std::cerr << "Error getting device info for device " << i << ": " << status << std::endl;
        }
    }
}