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

//Serial number big= 1118880

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
    Sleep(200); // Windows Sleep in milliseconds (200 milliseconds)
#else
    usleep(200000); // Unix usleep in microseconds (200 milliseconds)
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
    
 // ✅ Initialize high byte - Temperature sensor chip select
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState &= ~TSCS; // Start with TS chip select LOW (inactive)
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // ✅ Initialize low byte - Clock and data lines
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    LowByteHiLowState |= CLKSTATE;  // Clock HIGH (idle state)
    LowByteHiLowState |= DATASTATE; // Data HIGH (idle state)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // ✅ Activate temperature sensor - Take chip select HIGH
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState |= TSCS; // Take TS chip select HIGH (active)
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // ✅ Small delay for sensor activation
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // ✅ Send temperature read command (0x01 for most temp sensors)
    tx[pos++] = CMD_CLOCK_OUT_BITS_MSB;
    tx[pos++] = 0x07; // 7 = 8 bits - 1
    tx[pos++] = TSCMD; // Use defined temperature command (0xE0)

    // ✅ Switch to input mode to read data
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = INPUTMODE;

    // ✅ Read 2 bytes from temperature sensor
    tx[pos++] = CMD_CLOCK_IN_BYTES_MSB;
    tx[pos++] = LENGTH_2_BYTES;  // Read 2 bytes
    tx[pos++] = LENGTH_1_BYTE;   // High byte = 0 (total = 2 bytes)

    // ✅ Deactivate temperature sensor - Take chip select LOW
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    HighByteHiLowState &= ~TSCS; // Take TS chip select LOW (inactive)
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // ✅ Send the command string
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

void DIOHandler::testread(){
    if (!m_isDeviceOpen || !m_isMpsseOn) {
        Debug.Error("Device not open or MPSSE not enabled for test read.");
        return;
    }

    Debug.Log("Starting basic device read test...");

    // Read GPIO status - simplest possible read operation
    unsigned char tx[10], rx[10];
    DWORD ret_bytes;
    int pos = 0;

    // Command to read low byte GPIO status
    tx[pos++] = 0x81; // Read Data Bits Low Byte command

    // Send the command
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Test read: Failed to send GPIO read command, status: ", status);
        return;
    }

    Debug.Log("Test read: GPIO read command sent successfully");

    // Small delay
#ifdef _WIN32
    Sleep(100); // 100ms delay
#else
    usleep(100000);
#endif

    // Read the response
    status = FT_Read(ftHandle, rx, 1, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Test read: Failed to read GPIO data, status: ", status);
        return;
    }

    if (ret_bytes < 1) {
        Debug.Error("Test read: No data bytes returned: ", ret_bytes);
        return;
    }

    // Log the raw GPIO state
    Debug.Log("Test read SUCCESS: GPIO Low Byte = 0x" + 
              std::string(1, "0123456789ABCDEF"[rx[0] >> 4]) + 
              std::string(1, "0123456789ABCDEF"[rx[0] & 0x0F]) + 
              " (decimal: " + std::to_string((int)rx[0]) + ")");

    // Test reading high byte as well
    pos = 0;
    tx[pos++] = 0x83; // Read Data Bits High Byte command
    
    status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status == FT_OK) {
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
        status = FT_Read(ftHandle, rx, 1, &ret_bytes);
        if (status == FT_OK && ret_bytes >= 1) {
            Debug.Log("Test read SUCCESS: GPIO High Byte = 0x" + 
                      std::string(1, "0123456789ABCDEF"[rx[0] >> 4]) + 
                      std::string(1, "0123456789ABCDEF"[rx[0] & 0x0F]) + 
                      " (decimal: " + std::to_string((int)rx[0]) + ")");
        }
    }

    Debug.Log("Basic device read test completed.");
}

void DIOHandler::testread2(){
    Debug.Log("=== STARTING BASIC DEVICE TEST ===");
    
    // ✅ Enhanced Test 1: Check device handle and detailed status
    Debug.Log("TEST 1: Checking device handle...");
    if (ftHandle == NULL) {
        Debug.Error("TEST FAILED: Device handle is NULL - device not opened");
        Debug.Log("Check if you called connectMiniX() successfully first");
        return;
    }
    Debug.Log("TEST PASS: Device handle is not NULL");
    Debug.Log("Device handle value: " + std::to_string((uintptr_t)ftHandle));
    
    // ✅ Additional device info checks
    Debug.Log("TEST 1b: Checking device state variables...");
    Debug.Log("m_isDeviceOpen: " + std::string(m_isDeviceOpen ? "true" : "false"));
    Debug.Log("m_isMpsseOn: " + std::string(m_isMpsseOn ? "true" : "false"));
    Debug.Log("m_minixDeviceFound: " + std::string(m_minixDeviceFound ? "true" : "false"));
    
    // ✅ Test 1c: Try to get device info
    Debug.Log("TEST 1c: Trying to get device information...");
    char serialNumber[16] = {0};
    char description[64] = {0};
    FT_STATUS infoStatus = FT_GetDeviceInfo(ftHandle, NULL, NULL, serialNumber, description, NULL);
    if (infoStatus == FT_OK) {
        Debug.Log("TEST PASS: Device info accessible");
        Debug.Log("Serial: " + std::string(serialNumber));
        Debug.Log("Description: " + std::string(description));
    } else {
        Debug.Error("TEST WARNING: Cannot get device info - error: " + std::to_string(infoStatus));
        Debug.Log("This suggests the device handle might be invalid");
    }
    
    // ✅ Enhanced Test 2: Check device status with more details
    Debug.Log("TEST 2: Checking device modem status...");
    DWORD modemStatus = 0;
    FT_STATUS status = FT_GetModemStatus(ftHandle, &modemStatus);
    if (status != FT_OK) {
        Debug.Error("TEST FAILED: Cannot get device status - error: " + std::to_string(status));
        
        // Provide detailed error analysis
        switch(status) {
            case FT_INVALID_HANDLE:
                Debug.Error("Error Analysis: FT_INVALID_HANDLE - Device handle is invalid");
                Debug.Log("Solution: Check if device was properly opened");
                break;
            case FT_DEVICE_NOT_FOUND:
                Debug.Error("Error Analysis: FT_DEVICE_NOT_FOUND - Device disconnected");
                Debug.Log("Solution: Check USB connection and reconnect device");
                break;
            case FT_DEVICE_NOT_OPENED:
                Debug.Error("Error Analysis: FT_DEVICE_NOT_OPENED - Device not opened");
                Debug.Log("Solution: Call connectMiniX() first");
                break;
            case FT_IO_ERROR:
                Debug.Error("Error Analysis: FT_IO_ERROR - Communication error");
                Debug.Log("Solution: Check drivers and USB connection");
                break;
            default:
                Debug.Error("Error Analysis: Unknown FTDI error code: " + std::to_string(status));
        }
        
        // Try to diagnose further
        Debug.Log("DIAGNOSIS: Attempting to check if device is still connected...");
        DWORD numDevices = 0;
        FT_STATUS listStatus = FT_CreateDeviceInfoList(&numDevices);
        if (listStatus == FT_OK) {
            Debug.Log("Device list status: " + std::to_string(numDevices) + " FTDI devices found");
            if (numDevices == 0) {
                Debug.Error("DIAGNOSIS: No FTDI devices found - device may be disconnected");
            }
        } else {
            Debug.Error("DIAGNOSIS: Cannot even list FTDI devices - driver issue?");
        }
        
        return;
    }
    Debug.Log("TEST PASS: Device status readable, modem status = 0x" + 
              std::string(1, "0123456789ABCDEF"[(modemStatus >> 4) & 0xF]) + 
              std::string(1, "0123456789ABCDEF"[modemStatus & 0xF]));
    
    // ✅ Test 3: Check how many bytes are in receive queue
    DWORD bytesInQueue = 0;
    status = FT_GetQueueStatus(ftHandle, &bytesInQueue);
    if (status != FT_OK) {
        Debug.Error("TEST FAILED: Cannot check queue status - error: " + std::to_string(status));
        return;
    }
    Debug.Log("TEST INFO: Bytes in receive queue = " + std::to_string(bytesInQueue));
    
    // ✅ Clear any existing data in the queue
    if (bytesInQueue > 0) {
        unsigned char clearBuffer[100];
        DWORD bytesRead = 0;
        FT_Read(ftHandle, clearBuffer, bytesInQueue > 100 ? 100 : bytesInQueue, &bytesRead);
        Debug.Log("TEST INFO: Cleared " + std::to_string(bytesRead) + " old bytes from queue");
    }
    
    // ✅ Test 4: Try the simplest possible write/read
    unsigned char testByte = 0x80; // Send "Bad Command" - device should respond with 0xFA 0x80
    DWORD bytesWritten = 0;
    
    status = FT_Write(ftHandle, &testByte, 1, &bytesWritten);
    if (status != FT_OK) {
        Debug.Error("TEST FAILED: Cannot write test byte - error: " + std::to_string(status));
        return;
    }
    
    if (bytesWritten != 1) {
        Debug.Error("TEST FAILED: Expected to write 1 byte, actually wrote " + std::to_string(bytesWritten));
        return;
    }
    Debug.Log("TEST PASS: Successfully wrote 1 test byte");
    
    // ✅ Test 5: Wait and check if device responded
#ifdef _WIN32
    Sleep(100); // Wait 100ms for response
#else
    usleep(100000);
#endif
    
    status = FT_GetQueueStatus(ftHandle, &bytesInQueue);
    if (status != FT_OK) {
        Debug.Error("TEST FAILED: Cannot check queue after write - error: " + std::to_string(status));
        return;
    }
    
    Debug.Log("TEST INFO: After write, bytes in queue = " + std::to_string(bytesInQueue));
    
    if (bytesInQueue == 0) {
        Debug.Error("TEST FAILED: Device did not respond to test command");
        Debug.Log("This could mean:");
        Debug.Log("  1. Device is not in MPSSE mode");
        Debug.Log("  2. Device is not properly connected");
        Debug.Log("  3. Wrong device type");
        Debug.Log("  4. Hardware issue");
        return;
    }
    
    // ✅ Test 6: Read the response
    unsigned char responseBuffer[10];
    DWORD bytesRead = 0;
    
    status = FT_Read(ftHandle, responseBuffer, bytesInQueue, &bytesRead);
    if (status != FT_OK) {
        Debug.Error("TEST FAILED: Cannot read response - error: " + std::to_string(status));
        return;
    }
    
    Debug.Log("TEST SUCCESS: Read " + std::to_string(bytesRead) + " bytes from device");
    
    // ✅ Display the raw response data
    std::string hexData = "Response data: ";
    for (DWORD i = 0; i < bytesRead; i++) {
        char hexByte[4];
        sprintf(hexByte, "%02X ", responseBuffer[i]);
        hexData += hexByte;
    }
    Debug.Log(hexData);
    
    // ✅ Check if we got expected "Bad Command" response
    if (bytesRead >= 2 && responseBuffer[0] == 0xFA && responseBuffer[1] == 0x80) {
        Debug.Log("TEST SUCCESS: Device responded correctly to bad command (0xFA 0x80)");
        Debug.Log("=== DEVICE COMMUNICATION IS WORKING ===");
        Debug.Log("Your FTDI device is properly connected and responding");
        Debug.Log("The issue with temperature reading is likely in the protocol/timing");
    } else if (bytesRead > 0) {
        Debug.Log("TEST PARTIAL SUCCESS: Device is responding but not with expected bad command response");
        Debug.Log("This might be OK - device could be in different mode or different firmware");
        Debug.Log("At least basic communication is working");
    } else {
        Debug.Log("TEST WARNING: Got response but no readable data");
    }
    
    // ✅ Test 7: Try a simple GPIO read (like your testread() function)
    Debug.Log("TEST INFO: Attempting GPIO read test...");
    
    unsigned char gpioCmd = 0x81; // Read GPIO Low Byte
    bytesWritten = 0;
    
    status = FT_Write(ftHandle, &gpioCmd, 1, &bytesWritten);
    if (status == FT_OK && bytesWritten == 1) {
#ifdef _WIN32
        Sleep(50);
#else
        usleep(50000);
#endif
        
        status = FT_GetQueueStatus(ftHandle, &bytesInQueue);
        if (status == FT_OK && bytesInQueue > 0) {
            unsigned char gpioData = 0;
            status = FT_Read(ftHandle, &gpioData, 1, &bytesRead);
            if (status == FT_OK && bytesRead == 1) {
                Debug.Log("TEST SUCCESS: GPIO read returned: 0x" + 
                          std::string(1, "0123456789ABCDEF"[gpioData >> 4]) + 
                          std::string(1, "0123456789ABCDEF"[gpioData & 0xF]) + 
                          " (decimal: " + std::to_string((int)gpioData) + ")");
            }
        }
    }
    
    Debug.Log("=== BASIC DEVICE TEST COMPLETED ===");
    Debug.Log("If you see 'DEVICE COMMUNICATION IS WORKING' above,");
    Debug.Log("then your FTDI connection is good and the temperature");
    Debug.Log("issue is in the sensor-specific protocol.");
}

void DIOHandler::debugConnectionStatus() {
    Debug.Log("=== CONNECTION STATUS DEBUG ===");
    
    // Check global device state
    Debug.Log("Device State Variables:");
    Debug.Log("  m_isDeviceOpen: " + std::string(m_isDeviceOpen ? "true" : "false"));
    Debug.Log("  m_isMpsseOn: " + std::string(m_isMpsseOn ? "true" : "false"));
    Debug.Log("  m_minixDeviceFound: " + std::string(m_minixDeviceFound ? "true" : "false"));
    
    // Check controller state (if accessible)
    if (controller != nullptr) {
        Debug.Log("Controller State Variables:");
        Debug.Log("  controller->m_connectedToMinix: " + std::string(controller->m_connectedToMinix ? "true" : "false"));
        Debug.Log("  controller->m_tryingToConnectMinix: " + std::string(controller->m_tryingToConnectMinix ? "true" : "false"));
        Debug.Log("  controller->m_minixDeviceFound: " + std::string(controller->m_minixDeviceFound ? "true" : "false"));
        Debug.Log("  controller->m_minixOpened: " + std::string(controller->m_minixOpened ? "true" : "false"));
    }
    
    // Check device handle
    if (ftHandle != NULL) {
        Debug.Log("Device Handle: Valid (not NULL)");
        
        // Try basic device info
        char serial[16] = {0};
        char desc[64] = {0};
        FT_STATUS status = FT_GetDeviceInfo(ftHandle, NULL, NULL, serial, desc, NULL);
        if (status == FT_OK) {
            Debug.Log("  Serial: " + std::string(serial));
            Debug.Log("  Description: " + std::string(desc));
        } else {
            Debug.Error("  Cannot get device info - error: " + std::to_string(status));
        }
    } else {
        Debug.Error("Device Handle: NULL (not opened)");
    }
    
    // Check how many FTDI devices are available
    DWORD numDevices = 0;
    FT_STATUS listStatus = FT_CreateDeviceInfoList(&numDevices);
    if (listStatus == FT_OK) {
        Debug.Log("Total FTDI Devices Available: " + std::to_string(numDevices));
    } else {
        Debug.Error("Cannot enumerate FTDI devices - driver issue?");
    }
    
    Debug.Log("=== END CONNECTION STATUS DEBUG ===");
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