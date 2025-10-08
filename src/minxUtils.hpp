#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "Debug.hpp"
#include "ftd2xx.h"

// Mini-X Command Definitions for utilities
#define CMD_SET_CLK_DIVISOR         0x86    // Set CLK divisor

namespace MinixUtilities {

    // Function declarations (implementations in minxUtils.cpp)
    void setClockDivisor(unsigned char* tx, int& pos, int clockDivisor = 3);
    double convertToVoltage(unsigned char rx0, unsigned char rx1, double VRef = 4.096, double DAC_ADC_Scale = 4096.0, double HighVoltageConversionFactor = 10.0);
    double convertToCurrent(unsigned char rx0, unsigned char rx1, double VRef = 4.096, double DAC_ADC_Scale = 4096.0, double CurrentConversionFactor = 50.0);
    double convertToTemperature(unsigned char MSB, unsigned char LSB, bool isF = false);

    // FTD2 utility functions
    FT_STATUS Write(FT_HANDLE handle, const unsigned char* data, DWORD size, DWORD* bytesWritten);
}