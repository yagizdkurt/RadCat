#include "minixc.hpp"
#include "App.hpp"
#include "DataIOHandler.hpp"
using namespace std;
using namespace MinixUtilities;
using namespace Utilities;

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


#pragma region Minix Read Methods

double MinixController::readVoltage() {
    if (!isDeviceOpen || !isMpsseOn) {Debug.Error("Device not open or MPSSE not enabled for voltage reading.");return -1.0;}

    unsigned char tx[100], rx[100]; DWORD ret_bytes; int pos = 0;

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
    if (status != FT_OK) {Debug.Error("HV ADC write command error: ", status);return -1.0;}
    if (status == FT_OK) {Debug.Log("HV ADC write command sent successfully.");}

    sleepMs(50);

    // Read the reply and checksum validation
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {Debug.Error("HV ADC read data status error: ", status);return -1.0;}
    if (ret_bytes < 2) {Debug.Error("HV ADC too few data bytes returned: ", ret_bytes);return -1.0;}
    if (!Utilities::validateAndLogData(rx, 2, "HV ADC")) {return -1.0;}

    // Convert ADC result to voltage (bit manipulation handled in utility)
    double voltage = MinixUtilities::convertToVoltage(rx[0], rx[1]);

    Debug.Log("Read voltage: " + std::to_string(voltage) + " kV");
    return voltage;
}

double MinixController::readCurrent() {
    if (!isDeviceOpen || !isMpsseOn) {Debug.Error("Device not open or MPSSE not enabled for current reading.");return -1.0;}

    unsigned char tx[100], rx[100]; DWORD ret_bytes; int pos = 0;

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
    if (status != FT_OK) {Debug.Error("Current ADC write command error: ", status);return -1.0;}
    if constexpr (minixDebug) Debug.Log("Current ADC write command sent successfully.");
    sleepMs(200);

    // Read the reply
    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {Debug.Error("Current ADC read data status error: ", status);return -1.0;}
    if (ret_bytes < 2) {Debug.Error("Current ADC too few data bytes returned: ", ret_bytes);return -1.0;}
    if (!Utilities::validateAndLogData(rx, 2, "Current ADC")) {return -1.0;}

    double current = MinixUtilities::convertToCurrent(rx[0], rx[1]);

    Debug.Log("Read current: " + std::to_string(current) + " uA");
    return current;
}

double MinixController::readTemperature() {
    if (!isDeviceOpen || !isMpsseOn) {Debug.Error("Device not open or MPSSE not enabled for temperature reading.");return -1.0;}

    unsigned char tx[100], rx[100]; DWORD ret_bytes; int pos = 0;

    setClockDivisor(tx, pos, 3);
    
    // Set TS chip select low (prepare for communication)
    tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
    CLEAR(HighByteHiLowState, TSCS);  // TS chip select low
    tx[pos++] = HighByteHiLowState;
    tx[pos++] = OUTPUTMODE_H;

    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    CLEAR(LowByteHiLowState, CLKSTATE);
    CLEAR(LowByteHiLowState, DATASTATE);
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    activateTemperatureSensor(tx, pos, HighByteHiLowState);

    tx[pos++] = CMD_SET_DATA_BITS_LOWBYTE;
    SET(LowByteHiLowState, CLKSTATE);
    CLEAR(LowByteHiLowState, DATASTATE);
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;

    // Read cmd
    tx[pos++] = 0x12; // Clock out bits, MSB first (different from 0x13!)
    tx[pos++] = 0x07; // 7 = 8 bits - 1
    tx[pos++] = 0x01; // CORRECT temp sensor command (not 0xE0!)

    // Read 2 bytes from temperature sensor (CORRECT COMMAND!)
    tx[pos++] = 0x20; // Clock IN bytes, MSB first (not CMD_CLOCK_IN_BYTES_MSB!)
    tx[pos++] = LENGTH_2_BYTES;  // Read 2 bytes
    tx[pos++] = LENGTH_1_BYTE;   // High byte = 0 (total = 2 bytes)

    deactivateTemperatureSensor(tx, pos, HighByteHiLowState);


    purgeBuffers(ftHandle);
    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) {Debug.Error("Temperature sensor write command error: ", status);return -1.0;}
    if constexpr (minixDebug) Debug.Log("Temperature sensor write command sent successfully.");
    sleepMs(50);

    status = FT_Read(ftHandle, rx, 2, &ret_bytes);
    if (status != FT_OK) {Debug.Error("Temperature sensor read data status error: ", status);return -1.0;}
    if (ret_bytes < 2) {Debug.Error("Temperature sensor too few data bytes returned: ", ret_bytes);return -1.0;}
    if (!Utilities::validateAndLogData(rx, 2, "Temperature Sensor")) {return -1.0;}

    // Process temperature result (different from ADC - direct MSB/LSB)
    double temperature = MinixUtilities::convertToTemperature(rx[1], rx[0], false); // Celsius

    if constexpr (minixDebug) Debug.Log("Read temperature: " + std::to_string(temperature) + " C");
    return temperature;
}

void MinixController::testread(){
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

#pragma endregion




#pragma region Minix Write Methods

void MinixController::setVoltage(double voltage) {
    // Voltage setting implementation - placeholder TODO: Add actual voltage setting code
    Debug.Log("Setting voltage to: " + std::to_string(voltage) + " kV");
}

void MinixController::setCurrent(double current) {
    // Current setting implementation - placeholder TODO: Add actual current setting code
    Debug.Log("Setting current to: " + std::to_string(current) + " uA");
}

#pragma endregion




#pragma region MinixSetup and close Methods

bool MinixController::disconnectMiniX(){
    if constexpr (minixDebug) Debug.Log("Disconnecting from MiniX...");
    closeDevice();
    return true;
}

bool MinixController::closeDevice() {
    if (isDeviceOpen) {
        purgeBuffers(ftHandle);
        FT_ResetDevice(ftHandle); // Reset device before closing
        if (FT_Close(ftHandle) != FT_OK) {Debug.Error("Error closing device.");return false;}
        isDeviceOpen = false;
        if constexpr (minixDebug) Debug.Log("Device closed.");
    } else {
        if constexpr (minixDebug) Debug.Log("No device is open.");
    }
    return true;
}

int MinixController::getConnectedDevices() {
    if (isDeviceOpen) {Debug.Warn("Device is already open. Not fatal but a bug. Probably from UI.");return -1;}
    FT_STATUS status; DWORD numDevs;

    // Get the number of connected devices
    status = FT_CreateDeviceInfoList(&numDevs);
    if (status != FT_OK) {Debug.Error("Error getting device list: " , status);return -1;}

    if constexpr (minixDebug) {
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
    }

    return numDevs;
}

bool MinixController::findMinixDevice(){
    int totalDeviceNumber = getConnectedDevices();
    if (totalDeviceNumber <= 0) {Debug.Error("No devices found to connect."); return false;}

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
            strncpy_s(minixSerialNumber, devInfo.SerialNumber, sizeof(minixSerialNumber));
            strncpy_s(minixDescription, devInfo.Description, sizeof(minixDescription));
            minixLocationId = devInfo.LocId;
            minixDeviceId = devInfo.ID;
            controller->m_minixDeviceFound = true;
            break;
        }
    }
    if (controller->m_minixDeviceFound) return true;
    else return false;
}

bool MinixController::initializeMiniX() {
    // Step 4: Setup temperature sensor
    if (!setupTemperatureSensor()) { Debug.Error("Failed to setup temperature sensor."); return false; }

    // Step 5: Set clock divisor
    if (!setupClockDivisor()) { Debug.Error("Failed to set clock divisor."); return false; }

    // Step 6: Initialize voltage and current to 0
    //setVoltage(0.0);
    //sleepMs(200); // Sleep for 200ms
    //setCurrent(0.0);

    if constexpr (minixDebug) Debug.Log("MiniX initialization completed successfully.");
    return true;
}

bool MinixController::openDevice(const char* serialNumber) {
    if (isDeviceOpen) {Debug.Warn("Device is already open. Not fatal but a bug.");return true;}

    FT_STATUS status = FT_OpenEx((PVOID)serialNumber, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

    if (status != FT_OK) {Debug.Error("Failed to open minx device: ", status);return false;}

    isDeviceOpen = true;
    if constexpr (minixDebug) Debug.Log("Device " + std::string(serialNumber) + " opened successfully.");
    FT_ResetDevice(ftHandle); // Reset device to ensure clean state
    FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX); // Clear RX and TX buffers
    sleepMs(100); // Wait for device to stabilize
    return true;
}

bool MinixController::openMPSSE() 
{
    unsigned char tx[100]; FT_STATUS status; DWORD ret_bytes;

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
    if (ret_bytes == 2 && rx[0] == 0xFA && rx[1] == 0xAA) {if constexpr (minixDebug) Debug.Log("Mini-X responded correctly to MPSSE test command.");} 
    else {Debug.Error("Unexpected response from Mini-X."); return false;}

    isMpsseOn = true;
    return true;
}

bool MinixController::setupClockDivisor() {
    unsigned char tx[10]; DWORD ret_bytes; int pos = 0;

    purgeBuffers(ftHandle);
    MinixUtilities::setClockDivisor(tx, pos, 3);

    FT_STATUS status = FT_Write(ftHandle, tx, pos, &ret_bytes);
    if (status != FT_OK) { Debug.Error("Error setting clock divisor: ", status); return false; }
    if constexpr (minixDebug) Debug.Log("Clock divisor set successfully.");
    return true;
}

bool MinixController::setupTemperatureSensor() {
    if (!isDeviceOpen || !isMpsseOn) {Debug.Error("Device not ready for temperature sensor setup");return false;}
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


#pragma region Interface Methods

bool MinixController::connectMiniX(){
    if constexpr (minixDebug) Debug.Log("Connecting to MiniX...");
    controller->m_tryingToConnectMinix = true;
    controller->m_minixDeviceFound = false;
    controller->m_minixOpened = false;

    //if Mini-X already connected skip, else try to find it
    if(!controller->m_minixDeviceFound) if(!findMinixDevice()){Debug.Error("No Mini-X device found among connected devices.");controller->m_tryingToConnectMinix = false;return false;}
    controller->m_minixDeviceFound = true;

    // Open the Mini-X device
    if(!openDevice(minixSerialNumber)){
        Debug.Error("Failed to open device with serial number: " + std::string(minixSerialNumber));
        controller->m_tryingToConnectMinix = false;
        return false;
    }
    if constexpr (minixDebug) Debug.Log("Device opened successfully.");
    controller->m_minixOpened = true;

    Sleep(200); // Wait for device to stabilize

    //MPSSE stuff
    if(!openMPSSE()){
        Debug.Error("Failed to open MPSSE on Mini-X.");
        controller->m_tryingToConnectMinix = false;
        FT_Close(ftHandle);
        controller->m_minixOpened = false;

        return false;
    }
    if constexpr (minixDebug) Debug.Log("MPSSE opened successfully.");
    controller->m_isMpsseOn = true;

    // Initialize Mini-X
    if(!initializeMiniX()){
        Debug.Error("Failed to initialize Mini-X.");
        controller->m_tryingToConnectMinix = false;
        return false;
    }
    if constexpr (minixDebug) Debug.Log("MiniX initialized successfully.");
    controller->m_connectedToMinix = true;
    controller->m_tryingToConnectMinix = false;
    return true;
}

#pragma endregion


