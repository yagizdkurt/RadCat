#include "minxUtils.hpp"

namespace MinixUtilities {

    void setClockDivisor(unsigned char* tx, int& pos, int clockDivisor) {
        tx[pos++] = CMD_SET_CLK_DIVISOR;    // Set CLK divisor command
        tx[pos++] = clockDivisor & 0xFF;           // CLK divisor value (low byte)
        tx[pos++] = (clockDivisor >> 8) & 0xFF;    // CLK divisor high byte
        Debug.Log("Set clock divisor to: 1/" + std::to_string(clockDivisor));
    }

    double convertToVoltage(unsigned char rx0, unsigned char rx1, double VRef, double DAC_ADC_Scale, double HighVoltageConversionFactor) {
        // Extract 12-bit ADC value using original main.cpp bit manipulation
        unsigned char msb = rx0 >> 3;
        unsigned char lsb = ((rx0 & 0x07) << 5) | (rx1 >> 3);
        int ivolts = (msb << 8) | lsb;
        
        // Convert ADC value to voltage using the reference voltage and scale
        double voltage = (static_cast<double>(ivolts) / DAC_ADC_Scale * VRef) * HighVoltageConversionFactor;

        Debug.Log("MSB: " + std::to_string(msb) + ", LSB: " + std::to_string(lsb) + 
                  ", ADC Value: " + std::to_string(ivolts) + 
                  ", Converted Voltage: " + std::to_string(voltage) + "V");

        return voltage;
    }

    double convertToCurrent(unsigned char rx0, unsigned char rx1, double VRef, double DAC_ADC_Scale, double CurrentConversionFactor) {
        // Extract 12-bit ADC value using original main.cpp bit manipulation
        unsigned char msb = rx0 >> 3;
        unsigned char lsb = ((rx0 & 0x07) << 5) | (rx1 >> 3);
        int ivolts = (msb << 8) | lsb;
        
        // Convert ADC value to voltage first, then to current
        double current = (static_cast<double>(ivolts) / DAC_ADC_Scale * VRef) * CurrentConversionFactor;

        Debug.Log("MSB: " + std::to_string(msb) + ", LSB: " + std::to_string(lsb) + 
                  ", ADC Value: " + std::to_string(ivolts) + 
                  ", Converted Current: " + std::to_string(current) + "µA");

        return current;
    }

    double convertToTemperature(unsigned char MSB, unsigned char LSB, bool isF) {
        // Extract 12-bit temperature value using original main.cpp logic
        // Temperature sensor uses 12-bit resolution: (MSB<<4) + (LSB>>4)
        int tempRaw = (MSB << 4) + (LSB >> 4);
        
        double temp_c;
        
        // Check if the temperature is negative (MSB >= 0x80)
        if (MSB >= 0x80) {
            // If sign bit is set, temp is negative - subtract 4096 for 12-bit signed
            temp_c = static_cast<double>(tempRaw - 4096) * 0.0625;
        } else {
            temp_c = static_cast<double>(tempRaw) * 0.0625;
        }
        
        // Convert to Fahrenheit if requested
        double temperature = isF ? (temp_c * 9.0 / 5.0 + 32.0) : temp_c;

        Debug.Log("MSB: " + std::to_string(MSB) + ", LSB: " + std::to_string(LSB) + 
                  ", Raw 12-bit: " + std::to_string(tempRaw) + 
                  ", Temperature: " + std::to_string(temperature) + 
                  (isF ? "°F" : "°C"));

        return temperature;
    }

    FT_STATUS Write(FT_HANDLE handle, const unsigned char* data, DWORD size, DWORD* bytesWritten) {
        // Wrapper for FT_Write with logging
        FT_STATUS status = FT_Write(handle, (LPVOID)data, size, bytesWritten);
        if (status != FT_OK) {
            Debug.Error("FT_Write failed with status: ", status);
        } else {
            Debug.Log("FT_Write succeeded, bytes written: " + std::to_string(*bytesWritten));
        }
        return status;
    }

}