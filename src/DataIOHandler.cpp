#include "ftd2xx.h"
#include "DataIOHandler.hpp"
#include "App.hpp" 
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"
#ifdef _WIN32
    #include <windows.h>
    #include <cstring>       // For strncpy, strstr
#else
    #include <unistd.h>
    #include <cstring>
#endif

#pragma region byte Definitions

// Byte Definitions for Mini-X control

// MPSSE Commands
#define CMD_SET_DATA_BITS_LOWBYTE   0x80    // Set Data Bits Low Byte
#define CMD_SET_DATA_BITS_HIGHBYTE  0x82    // Set Data Bits High Byte
#define CMD_SET_CLK_DIVISOR         0x86    // Set CLK divisor
#define CMD_CLOCK_OUT_BITS_MSB      0x13    // Clock out bits, MSB first
#define CMD_CLOCK_IN_BYTES_MSB      0x20    // Clock in bytes, MSB first
#define CMD_CLOCK_OUT_BYTES_NEG     0x10    // Clock out bytes on negative edge, MSB first

// Pin/Port Definitions
#define OUTPUTMODE                  0x7B    // Output mode mask
#define INPUTMODE                   OUTPUTMODE
#define OUTPUTMODE_H                0x08    // High byte output mode
#define INPUTMODE_H                 OUTPUTMODE_H

// Chip Selects and Control Pins
#define TSCS                        0x08    // Temperature sensor chip select (high byte)
#define ADCS                        0x08    // ADC chip select
#define DACS                        0x10    // DAC chip select
#define CTRL_HV_EN_A                0x20    // HV Enable A
#define CTRL_HV_EN_B                0x40    // HV Enable B
#define DATASTATE                   0x02    // Data state when configured as output
#define CLKSTATE                    0x01    // Clock state

// DAC/ADC Channel Addresses
#define DACA                        0x18    // DAC Channel A
#define DACB                        0x19    // DAC Channel B
#define AD0                         0xD0    // ADC Channel 0 (Voltage)
#define AD1                         0xF0    // ADC Channel 1 (Current)

// GPIO Register Offsets
#define GPIO_LOW                    0x00    // Low byte GPIO register
#define GPIO_HIGH                   0x02    // High byte GPIO register

// Temperature Sensor Commands
#define TSCMD                       0xE0    // Temperature sensor command
#define TSSTATUS                    0x00    // Temperature sensor status
#define TSLSB                       0x01    // Temperature sensor LSB
#define TSMSB                       0x02    // Temperature sensor MSB
#define TSCONFIG                    0x80    // Temperature sensor config

// Clock and Timing
#define CLK_DIVISOR_LOW             0x03    // Clock divisor low byte
#define CLK_DIVISOR_HIGH            0x00    // Clock divisor high byte

// Length Definitions
#define LENGTH_1_BYTE               0x00    // 0=1byte
#define LENGTH_2_BYTES              0x01    // 1=2bytes
#define LENGTH_4_BITS               0x03    // 3=4bits

#pragma endregion

#pragma region minix device info

// Mini-X Device Information Storage
char m_minixSerialNumber[16];      // Store serial number (stable identifier)
char m_minixDescription[64];       // Store device description
DWORD m_minixLocationId;           // Store location ID (USB port)
DWORD m_minixDeviceId;             // Store device/product ID
bool m_minixDeviceFound = false;   // Track if Mini-X was found

#pragma endregion


bool DIOHandler::dataHandlerInit(){
    buffer = (char*)malloc(sizeof(char)*socketBufferSize);
    if(buffer == nullptr){Debug.Error("Failed to allocate memory for UDP buffer.");return false;}


    return true;
}


//INTERFACE METHODS
void DIOHandler::connectMiniX(){
    Debug.Log("Connecting to MiniX...");
    controller->m_tryingToConnectMinix = true;
    controller->m_minixDeviceFound = false;
    controller->m_minixOpened = false;

    int totalDeviceNumber = getConnectedDevices();
    if (totalDeviceNumber <= 0) {Debug.Error("No devices found to connect.");controller->m_tryingToConnectMinix = false;return;}

    // Try to find the Mini-X device among connected devices
    for (int i = 0; i < totalDeviceNumber; i++) {
        // Retrieve device info
        FT_DEVICE_LIST_INFO_NODE devInfo;
        ftStatus = FT_GetDeviceInfoDetail(i, &devInfo.Flags, &devInfo.Type, &devInfo.ID,
                                         &devInfo.LocId, devInfo.SerialNumber,
                                         devInfo.Description, &devInfo.ftHandle);
        if (ftStatus != FT_OK) {
            Debug.Error("Error getting device info for device " + std::to_string(i) + ": ", ftStatus);
            continue;
        }

        // Check if description contains "Mini-X" or "AMPTEK"
        if (strstr(devInfo.Description, "Mini-X") != nullptr || 
            strstr(devInfo.Description, "AMPTEK") != nullptr || devInfo.ID == 67358808) { 
            Debug.Log("Found Mini-X device: " + std::string(devInfo.Description));
            // Store device information
            strncpy(m_minixSerialNumber, devInfo.SerialNumber, sizeof(m_minixSerialNumber));
            strncpy(m_minixDescription, devInfo.Description, sizeof(m_minixDescription));
            m_minixLocationId = devInfo.LocId;
            m_minixDeviceId = devInfo.ID;
            m_minixDeviceFound = true;
            break;
        }
    }

    if (!m_minixDeviceFound) {Debug.Error("No Mini-X device found among connected devices.");controller->m_tryingToConnectMinix = false;return;}
    controller->m_minixDeviceFound = true;

    if (openDevice(m_minixSerialNumber)) {
        Debug.Log("Device opened successfully.");
        controller->m_minixOpened = true;
        if (initializeMiniX()) {
            Debug.Log("MiniX initialized successfully.");
            controller->m_connectedToMinix = true;
        } else {
            Debug.Error("Failed to initialize MiniX.");
        }
    } else {
        Debug.Error("Failed to open device.");
    }
    controller->m_tryingToConnectMinix = false;
}

void DIOHandler::disconnectMiniX(){
    Debug.Log("Disconnecting from MiniX...");
    closeDevice();
}

int DIOHandler::getConnectedDevices() {
    if (m_isDeviceOpen) {
        Debug.Log("Device is already open.");
        return -1;
    }

    FT_STATUS status;
    DWORD numDevs;

    // Get the number of connected devices
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status != FT_OK) { 
        Debug.Error("Error getting device list: " , status);
        return -1;
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
            return -1;
        }
    }
    return numDevs;
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

void DIOHandler::setVoltage(double voltage) {
    // Voltage setting implementation - placeholder TODO: Add actual voltage setting code
    Debug.Log("Setting voltage to: " + std::to_string(voltage) + " kV");
}

void DIOHandler::setCurrent(double current) {
    // Current setting implementation - placeholder TODO: Add actual current setting code
    Debug.Log("Setting current to: " + std::to_string(current) + " uA");
}

double DIOHandler::readVoltage() {
    if (!m_isDeviceOpen || !m_isMpsseOn) {
        Debug.Error("Device not open or MPSSE not enabled for voltage reading.");
        return -1.0;
    }

    unsigned char tx[100], rx[100];
    DWORD ret_bytes;
    int pos = 0;

    MinixUtilities::setClockDivisor(tx, pos);
    
    // Start condition - take ADC clock enable low
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState &= ~ADCS; // Clear ADCS bit (take ADC clock enable low)
    LowByteHiLowState &= ~CLKSTATE; // Clear CLKSTATE (take clock low)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Send control nibble - clock out 4 bits
    tx[pos++] = CMD_CLOCK_OUT_BITS_MSB;
    tx[pos++] = LENGTH_4_BITS;
    tx[pos++] = AD0; // Voltage channel

    // Set data direction to input
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = INPUTMODE;

    // Read 2 bytes from A/D conversion
    tx[pos++] = CMD_CLOCK_IN_BYTES_MSB;
    tx[pos++] = LENGTH_2_BYTES;
    tx[pos++] = LENGTH_1_BYTE; // LengthH

    // Take ADCS back high
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState |= ADCS; // Set ADCS bit (take ADC enable high)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

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

    // Hash/Checksum validation with automatic logging
    if (!Utilities::validateAndLogData(rx, 2, "HV ADC")) {return -1.0;}

    // Convert ADC result to voltage (bit manipulation handled in utility)
    double voltage = MinixUtilities::convertToVoltage(rx[0], rx[1]);

    Debug.Log("Read voltage: " + std::to_string(voltage) + " kV");
    return voltage;
}

double DIOHandler::readCurrent() {
    if (!m_isDeviceOpen || !m_isMpsseOn) {
        Debug.Error("Device not open or MPSSE not enabled for current reading.");
        return -1.0;
    }

    unsigned char tx[100], rx[100];
    DWORD ret_bytes;
    int pos = 0;

    MinixUtilities::setClockDivisor(tx, pos);
    
    // Start condition - take ADC clock enable low
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState &= ~ADCS; // Clear ADCS bit (take ADC clock enable low)
    LowByteHiLowState &= ~CLKSTATE; // Clear CLKSTATE (take clock low)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Send control nibble - clock out 4 bits
    tx[pos++] = CMD_CLOCK_OUT_BITS_MSB;
    tx[pos++] = LENGTH_4_BITS;
    tx[pos++] = AD1; // Current channel

    // Set data direction to input
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = INPUTMODE;

    // Read 2 bytes from A/D conversion
    tx[pos++] = CMD_CLOCK_IN_BYTES_MSB;
    tx[pos++] = LENGTH_2_BYTES;
    tx[pos++] = LENGTH_1_BYTE; // LengthH

    // Take ADCS back high
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState |= ADCS; // Set ADCS bit (take ADC enable high)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Send the command string
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Current ADC write command error: ", status);
        return -1.0;
    }
    Debug.Log("Current ADC write command sent successfully.");

    // Wait for conversion (longer for current reading)
#ifdef _WIN32
    Sleep(2000); // Windows Sleep in milliseconds (2 seconds)
#else
    usleep(2000000); // Unix usleep in microseconds (2 seconds)
#endif

    // Read the reply
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Current ADC read data status error: ", status);
        return -1.0;
    }

    if (ret_bytes < 2) {
        Debug.Error("Current ADC too few data bytes returned: ", ret_bytes);
        return -1.0;
    }

    // Hash/Checksum validation with automatic logging
    if (!Utilities::validateAndLogData(rx, 2, "Current ADC")) {
        return -1.0;
    }

    // Convert ADC result to current (bit manipulation handled in utility)
    double current = MinixUtilities::convertToCurrent(rx[0], rx[1]);

    Debug.Log("Read current: " + std::to_string(current) + " uA");
    return current;
}

double DIOHandler::readTemperature() {
    if (!m_isDeviceOpen || !m_isMpsseOn) {
        Debug.Error("Device not open or MPSSE not enabled for temperature reading.");
        return -1.0;
    }

    unsigned char tx[100], rx[100];
    DWORD ret_bytes;
    int pos = 0;

    MinixUtilities::setClockDivisor(tx, pos);
    
    // Start condition - take TS clock low
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState &= ~CLKSTATE; // Take clock low
    LowByteHiLowState &= ~DATASTATE; // Take data low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Set TS chip select high
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState |= TSCS; // Take chip select high
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // Setup for temperature sensor communication
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState &= ~CLKSTATE; // Take clock low
    LowByteHiLowState &= ~DATASTATE; // Take data low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState &= ~CLKSTATE; // Take clock low
    LowByteHiLowState &= ~DATASTATE; // Take data low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState |= CLKSTATE; // Take clock high
    LowByteHiLowState &= ~DATASTATE; // Keep data low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Send control address byte - clock out 8 bits
    tx[pos++] = CMD_CLOCK_OUT_BITS_MSB;
    tx[pos++] = 0x07; // 7 = 8 bits
    tx[pos++] = 0x01; // Temperature sensor command

    // Set data direction to input
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = INPUTMODE;

    // Read 2 bytes from temperature sensor
    tx[pos++] = CMD_CLOCK_IN_BYTES_MSB;
    tx[pos++] = LENGTH_2_BYTES;
    tx[pos++] = LENGTH_1_BYTE; // LengthH

    // Set TS chip select low
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState &= ~TSCS; // Take chip select low
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // Send the command string
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Temperature sensor write command error: ", status);
        return -1.0;
    }
    Debug.Log("Temperature sensor write command sent successfully.");

    // Wait for conversion
#ifdef _WIN32
    Sleep(200); // Windows Sleep in milliseconds
#else
    usleep(200000); // Unix usleep in microseconds
#endif

    // Read the reply
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Temperature sensor read data status error: ", status);
        return -1.0;
    }

    if (ret_bytes < 2) {
        Debug.Error("Temperature sensor too few data bytes returned: ", ret_bytes);
        return -1.0;
    }

    // Hash/Checksum validation with automatic logging
    if (!Utilities::validateAndLogData(rx, 2, "Temperature Sensor")) {
        return -1.0;
    }

    // Process temperature result (different from ADC - direct MSB/LSB)
    double temperature = MinixUtilities::convertToTemperature(rx[1], rx[0], false); // Celsius

    Debug.Log("Read temperature: " + std::to_string(temperature) + " C");
    return temperature;
}


# pragma region Minix Setup

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

bool DIOHandler::openDevice(const char* serialNumber) {
    if (m_isDeviceOpen) {Debug.Log("Device is already open.");return true;}

    FT_STATUS status = FT_OpenEx((PVOID)serialNumber, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    if (status != FT_OK) {Debug.Error("Failed to open minx device: ", status);return false;}

    m_isDeviceOpen = true;
    Debug.Log("Device " + std::string(serialNumber) + " opened successfully.");
    return true;
}

bool DIOHandler::setupGPIOLines() {
    unsigned char tx[20];
    DWORD ret_bytes;
    int pos = 0;

    // Setup MPSSE High byte I/O lines
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState = OUTPUTMODE_H;
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // Setup MPSSE Low byte I/O lines  
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState = 0xFB; // Initialize all high 1111 1011
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

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

    MinixUtilities::setClockDivisor(tx, pos, 3);

    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Error setting clock divisor: ", status);
        return false;
    }

    Debug.Log("Clock divisor set successfully.");
    return true;
}

bool DIOHandler::setupTemperatureSensor() {
    // Temperature sensor setup implementation TODO: Add actual setup code
    Debug.Log("Temperature sensor setup - placeholder implementation.");
    return true;
}


#pragma endregion