#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "Debug.hpp"
#include "ftd2xx.h"


#pragma region Bit Manipulation Macros
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

namespace MinixUtilities {

    // Function declarations (implementations in minxUtils.cpp)
    void setClockDivisor(unsigned char* tx, int& pos, int clockDivisor = 3);
    double convertToVoltage(unsigned char rx0, unsigned char rx1, double VRef = 4.096, double DAC_ADC_Scale = 4096.0, double HighVoltageConversionFactor = 10.0);
    double convertToCurrent(unsigned char rx0, unsigned char rx1, double VRef = 4.096, double DAC_ADC_Scale = 4096.0, double CurrentConversionFactor = 50.0);
    double convertToTemperature(unsigned char MSB, unsigned char LSB, bool isF = false);
    void purgeBuffers(FT_HANDLE handle);
    void SetBitsHigh(unsigned char* tx, int& pos, unsigned char bits);
    void SetBitsLow(unsigned char* tx, int& pos, unsigned char bits);


    // Temperature sensor control
    void activateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState);
    void deactivateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState);

    // FTD2 utility functions
    bool pollData(FT_HANDLE handle, DWORD bytesToRead, DWORD& bytesRead, int timeoutMs = 250);
}

