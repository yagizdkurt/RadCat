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
    
    // After getting devices, try to open the first one
    if (openDevice(0)) {
        Debug.Log("Device opened successfully.");
        if (initializeMiniX()) {
            Debug.Log("MiniX initialized successfully.");
        } else {
            Debug.Error("Failed to initialize MiniX.");
        }
    } else {
        Debug.Error("Failed to open device.");
    }
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


void DIOHandler::closeDevice() {
    if (m_isDeviceOpen) {
        FT_Close(ftHandle);
        m_isDeviceOpen = false;
        Debug.Log("Device closed.");
    } else {
        Debug.Log("No device is open.");
    }
}

bool DIOHandler::openDevice(int deviceIndex) {
    if (m_isDeviceOpen) {
        Debug.Log("Device is already open.");
        return true;
    }

    FT_STATUS status = FT_Open(deviceIndex, &ftHandle);
    if (status != FT_OK) {
        Debug.Error("Failed to open device " + std::to_string(deviceIndex) + ": ", status);
        return false;
    }

    m_isDeviceOpen = true;
    Debug.Log("Device " + std::to_string(deviceIndex) + " opened successfully.");
    return true;
}

bool DIOHandler::initializeMiniX() {
    if (!m_isDeviceOpen) {
        Debug.Error("No device is open for initialization.");
        return false;
    }

    // Step 1: Set latency timer and timeouts
    FT_STATUS status = FT_SetLatencyTimer(ftHandle, 4); // 4ms
    if (status != FT_OK) {
        Debug.Error("Failed to set latency timer: ", status);
        return false;
    }

    status = FT_SetTimeouts(ftHandle, 40, 40); // 40ms read/write timeouts
    if (status != FT_OK) {
        Debug.Error("Failed to set timeouts: ", status);
        return false;
    }

    // Step 2: Enable MPSSE mode
    status = FT_SetBitMode(ftHandle, 0x0, 0x02); // Enable MPSSE
    if (status != FT_OK) {
        Debug.Error("Failed to enable MPSSE mode: ", status);
        return false;
    }

    // Step 3: Initialize GPIO lines and hardware
    if (!setupGPIOLines()) {
        Debug.Error("Failed to setup GPIO lines.");
        return false;
    }

    // Step 4: Setup temperature sensor
    if (!setupTemperatureSensor()) {
        Debug.Error("Failed to setup temperature sensor.");
        return false;
    }

    // Step 5: Set clock divisor
    if (!setClockDivisor()) {
        Debug.Error("Failed to set clock divisor.");
        return false;
    }

    // Step 6: Initialize voltage and current to 0
    setVoltage(0.0);
#ifdef _WIN32
    Sleep(200); // Windows Sleep in milliseconds
#else
    usleep(200000); // Unix usleep in microseconds
#endif
    setCurrent(0.0);

    m_isMpsseOn = true;
    Debug.Log("MiniX initialization completed successfully.");
    return true;
}

bool DIOHandler::setupGPIOLines() {
    unsigned char tx[20];
    DWORD ret_bytes;
    int pos = 0;

    // Setup MPSSE High byte I/O lines
    tx[pos++] = 0x82; // Setup MPSSE High byte I/O lines
    HighByteHiLowState = 0x08; // OUTPUTMODE_H
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = 0x08; // OUTPUTMODE_H

    // Setup MPSSE Low byte I/O lines  
    tx[pos++] = 0x80; // Setup MPSSE Low byte I/O lines
    LowByteHiLowState = 0xFB; // Initialize all high 1111 1011
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = 0x7B; // OUTPUTMODE

    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Error initializing I/O lines: ", status);
        return false;
    }

    Debug.Log("GPIO lines initialized successfully.");
    return true;
}



bool DIOHandler::setClockDivisor() {
    unsigned char tx[10];
    DWORD ret_bytes;
    int pos = 0;

    tx[pos++] = 0x86; // Set CLK divisor
    tx[pos++] = 0x03; 
    tx[pos++] = 0x00;

    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Error setting clock divisor: ", status);
        return false;
    }

    Debug.Log("Clock divisor set successfully.");
    return true;
}

void DIOHandler::setVoltage(double voltage) {
    // Voltage setting implementation - placeholder TODO: Add actual voltage setting code
    Debug.Log("Setting voltage to: " + std::to_string(voltage) + " kV");
}

void DIOHandler::setCurrent(double current) {
    // Current setting implementation - placeholder TODO: Add actual current setting code
    Debug.Log("Setting current to: " + std::to_string(current) + " uA");
}

bool DIOHandler::setupTemperatureSensor() {
    // Temperature sensor setup implementation TODO: Add actual setup code
    Debug.Log("Temperature sensor setup - placeholder implementation.");
    return true;
}

double DIOHandler::readVoltage() {
    if (!m_isDeviceOpen || !m_isMpsseOn) {
        Debug.Error("Device not open or MPSSE not enabled for voltage reading.");
        return -1.0;
    }

    unsigned char tx[100], rx[100];
    DWORD ret_bytes;
    int pos = 0;

    // Set clock divisor
    tx[pos++] = 0x86; // Set CLK divisor
    tx[pos++] = 0x03; 
    tx[pos++] = 0x00;
    
    // Start condition - take ADC clock enable low
    tx[pos++] = 0x80; // Setup MPSSE Low byte I/O lines
    LowByteHiLowState &= ~0x08; // Clear ADCS bit (take ADC clock enable low)
    LowByteHiLowState &= ~0x01; // Clear CLKSTATE (take clock low)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = 0x7B; // OUTPUTMODE

    // Send control nibble - clock out 4 bits
    tx[pos++] = 0x13; // Clock out bits, MSB first
    tx[pos++] = 0x03; // 3 = 4bits
    tx[pos++] = 0xD0; // AD0: 1101 Start, Sngl/Diff, Odd/Sign, MSBF

    // Set data direction to input
    tx[pos++] = 0x80; // Setup MPSSE Low byte I/O lines
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = 0x7B; // INPUTMODE for data pin

    // Read 2 bytes from A/D conversion
    tx[pos++] = 0x20; // Clock in 1 byte, MSB first
    tx[pos++] = 0x01; // LengthL: 0=1byte, 1=2bytes
    tx[pos++] = 0x00; // LengthH

    // Take ADCS back high
    tx[pos++] = 0x80; // Setup MPSSE Low byte I/O lines
    LowByteHiLowState |= 0x08; // Set ADCS bit (take ADC enable high)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = 0x7B; // OUTPUTMODE

    // Send the command string
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("HV ADC write command error: ", status);
        return -1.0;
    }
    if (status == FT_OK) {
        Debug.Log("HV ADC write command sent successfully.");
    }

    // Wait for conversion
#ifdef _WIN32
    Sleep(200); // Windows Sleep in milliseconds
#else
    usleep(200000); // Unix usleep in microseconds
#endif

    // Read the reply
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("HV ADC read data status error: ", status);
        return -1.0;
    }

    if (ret_bytes < 2) {
        Debug.Error("HV ADC too few data bytes returned: ", ret_bytes);
        return -1.0;
    }

    // Process the ADC result
    unsigned char msb = rx[0] >> 3;
    unsigned char lsb = ((rx[0] & 0x07) << 5) | (rx[1] >> 3);
    int ivolts = (msb << 8) | lsb;
    
    // Convert to actual voltage
    // Using Mini-X conversion: VRef=4.096V, DAC_ADC_Scale=4096, HighVoltageConversionFactor=10.0
    double VRef = 4.096;
    double DAC_ADC_Scale = 4096.0;
    double HighVoltageConversionFactor = 10.0;
    
    double voltage = ((double)ivolts / DAC_ADC_Scale * VRef) * HighVoltageConversionFactor;
    
    Debug.Log("Read voltage: " + std::to_string(voltage) + " kV");
    return voltage;
}