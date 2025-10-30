#include "miniXDevice.hpp"
#include "ftd2xx.h"
#include "debug.hpp"
#include "utilities.hpp"
#include "minxUtils.hpp"

REGISTER_DEVICE(MiniXDevice, "Mini-X Device");
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

#define CLK_FN_NEG 0x10     //Clock out 2 bytes on negative edge, MSB first
#define CLK_FN CLK_FN_NEG

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

bool MiniXDevice::connect() {
    if (!connection.fConnect()) return false;
    if (!initializeGPIOs()) return false;
    return true;
}

bool MiniXDevice::disconnect() {
    return true;
}

void MiniXDevice::cycleCheck() {
    // Implement cycle check logic if needed
}

double MiniXDevice::readValue(const std::string& parameter) {
    // Implement reading values based on the parameter
    return 0.0;
}

bool MiniXDevice::setValue(const std::string& parameter, double value) {
    // Implement setting values based on the parameter
    return true;
}

bool MiniXDevice::initializeGPIOs() {
    unsigned char tx[100]; FT_STATUS status;
    //initialize clock, data, and digital I/O on Low-Byte, 8-bit port 
    int pos=0;
    // set TS chip select low
    tx[pos++] = 0x82;//Setup MPSSE High byte I/O lines
    HighByteHiLowState = OUTPUTMODE_H;
    CLEAR(HighByteHiLowState, TSCS);  //take chip select high
    tx[pos++] = HighByteHiLowState; //1110 0011  hi/low state if output (clock & data init to high)
    tx[pos++] = OUTPUTMODE_H;
    tx[pos++] = 0x80;//Setup MPSSE Low byte I/O lines
    LowByteHiLowState = 0xFB;//Initialize all high 1111 1011
    //LowByteHiLowState = OUTPUTMODE;//Initialize 20071203
    CLEAR(LowByteHiLowState, CTRL_HV_EN_A); //HVEN A disabled
    CLEAR(LowByteHiLowState, CTRL_HV_EN_B); //HVEN B disabled
    tx[pos++] = LowByteHiLowState;
    tx[pos++] = OUTPUTMODE;//0xFB
    status = connection.sendData(tx, pos);
    if(status != FT_OK){printf("Error initializing I/O lines.\n");return false;}
    return true;
}