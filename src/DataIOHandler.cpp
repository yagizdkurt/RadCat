#include "ftd2xx.h"
#include "DataIOHandler.hpp"
#include "App.hpp" 
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"

using namespace Utilities;
using namespace MinixUtilities;


#pragma region byte and pin Definitions

/*
LOW
Bit 0 (0x01): CLKSTATE     - SPI Clock signal
Bit 1 (0x02): DATASTATE    - SPI Data signal  
Bit 2 (0x04): [Unused/Reserved]
Bit 3 (0x08): ADCS         - ADC Chip Select
Bit 4 (0x10): DACS         - DAC Chip Select
Bit 5 (0x20): CTRL_HV_EN_A - High Voltage Enable A
Bit 6 (0x40): CTRL_HV_EN_B - High Voltage Enable B  
Bit 7 (0x80): [Unused/Reserved]
High
Bit 0 (0x01): [Unused/Reserved]
Bit 1 (0x02): [Unused/Reserved]
Bit 2 (0x04): [Unused/Reserved]
Bit 3 (0x08): TSCS         - Temperature Sensor Chip Select
Bit 4 (0x10): [Unused/Reserved]
Bit 5 (0x20): [Unused/Reserved]
Bit 6 (0x40): [Unused/Reserved]
Bit 7 (0x80): [Unused/Reserved]
*/

// Byte Definitions for Mini-X control

#define  SET(x, y)   ((x) |= (y)) 
#define  CLEAR(x, y)   ((x) &= (~y)) 

// MPSSE Commands
#define CMD_SET_DATA_BITS_LOWBYTE   0x80    // Set Data Bits Low Byte
#define CMD_SET_DATA_BITS_HIGHBYTE  0x82    // Set Data Bits High Byte
#define CMD_GET_DATA_BITS_LOWBYTE   0x81    // Get Data Bits Low Byte
#define CMD_GET_DATA_BITS_HIGHBYTE  0x83    // Get Data Bits High Byte
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

bool DIOHandler::dataHandlerInit(){

    return true;
}

//INTERFACE METHODS

void DIOHandler::disconnectMiniX(){
    Debug.Log("Disconnecting from MiniX...");
    closeDevice();
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

void DIOHandler::testread(){
    Debug.Log("=== MPSSE MODE TEST ===");
    unsigned char mpsseTestCmd = 0xAA; // Invalid command test - should return 0xFA 0xAA
    DWORD bytesWritten = 0; DWORD rxBytes = 0;

    purgeBuffers(ftHandle);
    FT_STATUS status = FT_Write(ftHandle, &mpsseTestCmd, 1, &bytesWritten);
    if (status != FT_OK) {Debug.Error("TEST FAILED: Cannot send MPSSE test command - error: " + std::to_string(status));return;}
    if (bytesWritten != 1) {Debug.Error("TEST FAILED: Expected to write 1 byte, wrote " + std::to_string(bytesWritten));return;}
    Debug.Log("TEST PASS: MPSSE test command sent successfully");

    if (!pollData(ftHandle, 2, rxBytes, 200)) return;

    unsigned char responseBuffer[10]; DWORD bytesRead = 0;

    status = FT_Read(ftHandle, responseBuffer, 2, &bytesRead);
    if (status != FT_OK) {Debug.Error("TEST FAILED: Cannot read MPSSE response - error: " + std::to_string(status));return;}
    Debug.Log("Read " + std::to_string(bytesRead) + " bytes from device");
    Debug.Log("=== MPSSE MODE TEST COMPLETED ===");
}

# pragma region Minix Setup




#pragma endregion

#pragma region Setup functions

bool DIOHandler::initializeMiniX() {
    // Step 4: Setup temperature sensor
    if (!setupTemperatureSensor()) {Debug.Error("Failed to setup temperature sensor.");return false;}

    // Step 5: Set clock divisor
    if (!setupClockDivisor()) {
        Debug.Error("Failed to set clock divisor.");
        return false;
    }

    // Step 6: Initialize voltage and current to 0
    //setVoltage(0.0);
    //sleepMs(200); // Sleep for 200ms
    //setCurrent(0.0);

    Debug.Log("MiniX initialization completed successfully.");
    return true;
}

bool DIOHandler::openDevice(const char* serialNumber) {
    if (m_isDeviceOpen) {Debug.Log("Device is already open.");return true;}

    FT_STATUS status = FT_OpenEx((PVOID)serialNumber, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    if (status != FT_OK) {Debug.Error("Failed to open minx device: ", status);return false;}

    m_isDeviceOpen = true;
    Debug.Log("Device " + std::string(serialNumber) + " opened successfully.");
    FT_ResetDevice(ftHandle); // Reset device to ensure clean state
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX); // Clear RX and TX buffers
    sleepMs(100); // Wait for device to stabilize
    return true;
}

bool DIOHandler::openMPSSE() 
{
    unsigned char tx[100];
    FT_STATUS status;
    DWORD ret_bytes;

    //Set USB Parameters and Latency Timer
    FT_SetUSBParameters(ftHandle, 65536, 65536); //Set USB request transfer sizes
    FT_SetLatencyTimer(ftHandle, 4); //4ms
    FT_SetTimeouts(ftHandle, 40, 40); //40ms read/write timeouts
    FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0); sleepMs(20);

    status = FT_SetBitMode(ftHandle, 0x0, 0x02);  //enable MPSSE 
    if(status != FT_OK){printf("Error enabling serial engine (MPSSE). Check controller connection.\n");return false;}
    sleepMs(100);
    purgeBuffers(ftHandle);

    //initialize clock, data, and digital I/O on Low-Byte, 8-bit port 
    int pos=0;
    // set TS chip select low
    tx[pos++] = 0x82;//Setup MPSSE High byte I/O lines
    HighByteHiLowState = OUTPUTMODE_H;
    CLEAR(HighByteHiLowState, TSCS);  //take chip select high
    tx[pos++] = HighByteHiLowState;//1110 0011  hi/low state if output (clock & data init to high)
    tx[pos++] = OUTPUTMODE_H;

    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    LowByteHiLowState = 0xFB;//Initialize all high 1111 1011
    //LowByteHiLowState = OUTPUTMODE;//Initialize 20071203
    CLEAR(LowByteHiLowState, CTRL_HV_EN_A); //HVEN A disabled
    CLEAR(LowByteHiLowState, CTRL_HV_EN_B); //HVEN B disabled
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;//0xFB
    status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if(status != FT_OK){printf("Error initializing I/O lines.\n");return false;}

    purgeBuffers(ftHandle);
    sleepMs(50);
    // Verify MPSSE mode by sending a test command and checking response
    unsigned char rx[10];
    tx[0] = 0xAA;
    FT_Write(ftHandle, tx, 1, &ret_bytes);
    sleepMs(50);
    FT_Read(ftHandle, rx, 2, &ret_bytes);
    // Expect: 0xFA 0xAA back
    if (ret_bytes == 2 && rx[0] == 0xFA && rx[1] == 0xAA) {Debug.Log("Mini-X responded correctly to MPSSE test command.");} 
    else {Debug.Error("Unexpected response from Mini-X."); return false;}

    m_isMpsseOn = true;
    return true;
}

bool DIOHandler::setupClockDivisor() {
    unsigned char tx[10];
    DWORD ret_bytes;
    int pos = 0;

    purgeBuffers(ftHandle);
    sleepMs(50);
    MinixUtilities::setClockDivisor(tx, pos, 3);

    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {
        Debug.Error("Error setting clock divisor: ", status);
        return false;
    }

    Debug.Log("Clock divisor set successfully.");
    return true;
}

void DIOHandler::connectMiniX(){
    Debug.Log("Connecting to MiniX...");
    controller->m_tryingToConnectMinix = true;
    controller->m_minixDeviceFound = false;
    controller->m_minixOpened = false;

    //if Mini-X already connected skip, else try to find it
    if(!m_minixDeviceFound) if(!findMinixDevice()){Debug.Error("No Mini-X device found among connected devices.");controller->m_tryingToConnectMinix = false;return;}
    controller->m_minixDeviceFound = true;

    // Open the Mini-X device
    if(!openDevice(m_minixSerialNumber)){
        Debug.Error("Failed to open device with serial number: " + std::string(m_minixSerialNumber));
        controller->m_tryingToConnectMinix = false;
        return;
    }
    Debug.Log("Device opened successfully.");
    controller->m_minixOpened = true;

    Sleep(200); // Wait for device to stabilize

    //MPSSE stuff
    if(!openMPSSE()){
        Debug.Error("Failed to open MPSSE on Mini-X.");
        controller->m_tryingToConnectMinix = false;
        FT_Close(ftHandle);
        controller->m_minixOpened = false;

        return;
    }
    Debug.Log("MPSSE opened successfully.");
    controller->m_isMpsseOn = true;

    // Initialize Mini-X
    if(!initializeMiniX()){
        Debug.Error("Failed to initialize Mini-X.");
        controller->m_tryingToConnectMinix = false;
        return;
    }
    Debug.Log("MiniX initialized successfully.");
    controller->m_connectedToMinix = true;
    controller->m_tryingToConnectMinix = false;

}

int DIOHandler::getConnectedDevices() {
    if (m_isDeviceOpen) {Debug.Log("Device is already open.");return -1;}
    FT_STATUS status; DWORD numDevs;

    // Get the number of connected devices
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status != FT_OK) {Debug.Error("Error getting device list: " , status);return -1;}
    Debug.Log("Number of connected devices: " , numDevs);

    //Debug devices open output
    /*
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
    */

    return numDevs;
}

bool DIOHandler::findMinixDevice(){
    int totalDeviceNumber = getConnectedDevices();
    if (totalDeviceNumber <= 0) {Debug.Error("No devices found to connect.");return false;}

    // Try to find the Mini-X device among connected devices
    for (int i = 0; i < totalDeviceNumber; i++) {
        // Retrieve device info
        FT_DEVICE_LIST_INFO_NODE devInfo;
        ftStatus = FT_GetDeviceInfoDetail(i,&devInfo.Flags, &devInfo.Type,&devInfo.ID,&devInfo.LocId,devInfo.SerialNumber,devInfo.Description, &devInfo.ftHandle);
        if (ftStatus != FT_OK) {Debug.Error("Error getting device info for device " + std::to_string(i) + ": ", ftStatus);continue;}

        // Check if description contains "Mini-X" or "AMPTEK"
        if (strstr(devInfo.Description, "Mini-X") != nullptr || 
            strstr(devInfo.Description, "AMPTEK") != nullptr || devInfo.ID == 67358808) { 
            Debug.Log("Found Mini-X device: " + std::string(devInfo.Description));
            // Store device information
            strncpy_s(m_minixSerialNumber, devInfo.SerialNumber, sizeof(m_minixSerialNumber));
            strncpy_s(m_minixDescription, devInfo.Description, sizeof(m_minixDescription));
            m_minixLocationId = devInfo.LocId;
            m_minixDeviceId = devInfo.ID;
            m_minixDeviceFound = true;
            controller->m_minixDeviceFound = true;
            break;
        }
    }
    if (m_minixDeviceFound) return true;
    else return false;
}

bool DIOHandler::setupTemperatureSensor() {
    if (!m_isDeviceOpen || !m_isMpsseOn) {Debug.Error("Device not ready for temperature sensor setup");return false;}
    unsigned char tx[100]; DWORD ret_bytes; int pos = 0;
    setClockDivisor(tx, pos, 0);

    //take TSCS low and clock 2 bytes of data into the Temp Sensor
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    CLEAR(LowByteHiLowState, CLKSTATE);//take CLock low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // set TS chip select high
    tx[pos++] = 0x82;//Setup MPSSE High byte I/O lines
    SET(HighByteHiLowState, TSCS);  //take chip select high
    tx[pos++] = HighByteHiLowState;//1110 0011  hi/low state if output (clock & data init to high)
    tx[pos++] = OUTPUTMODE_H;

    //take TSCS low and clock 2 bytes of data into the Temp Sensor
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    CLEAR(LowByteHiLowState, CLKSTATE);//take CLock low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    //take TSCS low and clock 2 bytes of data into the Temp Sensor
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    CLEAR(LowByteHiLowState, CLKSTATE);//take CLock low
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    //take TSCS low and clock 2 bytes of data into the Temp Sensor
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    SET(LowByteHiLowState, CLKSTATE);
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    tx[pos++] = 0x10;   //Clock out 2 bytes on positive edge, MSB first
    tx[pos++] = 0x01;   //LengthL 0=1byte, 1=2bytes
    tx[pos++] = 0x00;   //LengthH 
    tx[pos++] = TSCONFIG; //set configuration register
    tx[pos++] = TSCMD + 0x08;   // continuous convert, 12-bit res., no shutdown

    // set TS chip select low
    tx[pos++] = 0x82;//Setup MPSSE High byte I/O lines
    CLEAR(HighByteHiLowState, TSCS);  //take chip select low
    tx[pos++] = HighByteHiLowState;// 1110 0011  hi/low state if output (clock & data init to high)
    tx[pos++] = OUTPUTMODE_H;

    purgeBuffers(ftHandle);
    if(FT_Write(ftHandle, tx, pos, &ret_bytes) != FT_OK){Debug.Error("Temperature Sensor setup error"); return false;}
    return true;
}

#pragma endregion

#pragma region Read functions

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

    sleepMs(50);

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
    if (!m_isDeviceOpen || !m_isMpsseOn) {Debug.Error("Device not open or MPSSE not enabled for temperature reading.");return -1.0;}

    unsigned char tx[100], rx[100]; DWORD ret_bytes; int pos = 0;

    setClockDivisor(tx, pos);
    
    // ✅ Initialize high byte - Temperature sensor chip select
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    CLEAR(HighByteHiLowState, TSCS); // Start with TS chip select LOW (inactive)
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    // ✅ Initialize low byte - Clock and data lines LOW (critical!)
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    CLEAR(LowByteHiLowState, CLKSTATE);  // Clock LOW
    CLEAR(LowByteHiLowState, DATASTATE); // Data LOW (critical difference!)
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    activateTemperatureSensor(tx, pos, HighByteHiLowState);

    // ✅ Additional clock state setup (from working code)
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    CLEAR(LowByteHiLowState, CLKSTATE);  // Clock LOW
    CLEAR(LowByteHiLowState, DATASTATE); // Data LOW
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // ✅ More clock state setup 
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    CLEAR(LowByteHiLowState, CLKSTATE);  // Clock LOW
    CLEAR(LowByteHiLowState, DATASTATE); // Data LOW
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // ✅ Final clock setup before command
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    SET(LowByteHiLowState, CLKSTATE);   // Clock HIGH
    CLEAR(LowByteHiLowState, DATASTATE); // Data LOW
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // ✅ Send temperature read command (CORRECT COMMAND!)
    tx[pos++] = 0x12; // Clock out bits, MSB first (different from 0x13!)
    tx[pos++] = 0x07; // 7 = 8 bits - 1
    tx[pos++] = 0x01; // CORRECT temp sensor command (not 0xE0!)

    // ✅ Switch to input mode to read data
    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = INPUTMODE;

    // ✅ Read 2 bytes from temperature sensor (CORRECT COMMAND!)
    tx[pos++] = 0x20; // Clock IN bytes, MSB first (not CMD_CLOCK_IN_BYTES_MSB!)
    tx[pos++] = LENGTH_2_BYTES;  // Read 2 bytes
    tx[pos++] = LENGTH_1_BYTE;   // High byte = 0 (total = 2 bytes)

    deactivateTemperatureSensor(tx, pos, HighByteHiLowState);

    // ✅ Send the command string
    purgeBuffers(ftHandle);
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {Debug.Error("Temperature sensor write command error: ", status);return -1.0;}
    Debug.Log("Temperature sensor write command sent successfully.");
    sleepMs(50);

    // Read the reply and standard checks
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {Debug.Error("Temperature sensor read data status error: ", status);return -1.0;}
    if (ret_bytes < 2) {Debug.Error("Temperature sensor too few data bytes returned: ", ret_bytes);return -1.0;}
    if (!Utilities::validateAndLogData(rx, 2, "Temperature Sensor")) {return -1.0;}

    // Process temperature result (different from ADC - direct MSB/LSB)
    double temperature = MinixUtilities::convertToTemperature(rx[1], rx[0], false); // Celsius

    Debug.Log("Read temperature: " + std::to_string(temperature) + " C");
    return temperature;
}

#pragma endregion