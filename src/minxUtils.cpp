#include "minxUtils.hpp"
#include "utilities.hpp"
using namespace std;
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

    void purgeBuffers(FT_HANDLE handle){ // Purge both RX and TX buffers
        FT_STATUS status = FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
        Utilities::sleepMs(20); // Small delay to ensure purge completes
        Debug.Log("Purged RX and TX buffers.");
    }


    // Temperature sensor control
    void activateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState) {
        // Activate temperature sensor by setting its chip select low
        tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
        CLEAR(HighByteHiLowState, TSCS);  // Set TSCS low to activate temperature sensor
        tx[pos++] = HighByteHiLowState;
        tx[pos++] = OUTPUTMODE_H;
        Debug.Log("Activated temperature sensor (TSCS low).");
    }

    void deactivateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState) {
        // Deactivate temperature sensor by setting its chip select high
        tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
        SET(HighByteHiLowState, TSCS);  // Set TSCS high to deactivate temperature sensor
        tx[pos++] = HighByteHiLowState;
        tx[pos++] = OUTPUTMODE_H;
        Debug.Log("Deactivated temperature sensor (TSCS high).");
    }

    // FTD2 utility functions
    bool pollData(FT_HANDLE handle, DWORD bytesToRead, DWORD& bytesRead, int timeoutMs) {
        if(handle == nullptr || bytesToRead == 0) {Debug.Error("PollData: Invalid handle or bytesToRead."); return false;}
        if(timeoutMs <= 0) {Debug.Error("PollData: timeout must be positive."); return false;}
        DWORD rxBytes = 0; bytesRead = 0; int elapsed = 0; const int pollInterval = 20;
        
        while (bytesRead < bytesToRead && elapsed < timeoutMs) {
            FT_GetQueueStatus(handle, &rxBytes);
            if (rxBytes > 0) {
                bytesRead = rxBytes;
                if (bytesRead > bytesToRead) { bytesRead = bytesToRead; }
            }
            Utilities::sleepMs(pollInterval);
            elapsed += pollInterval;
        }
        if (bytesRead < bytesToRead) { Debug.Warn("PollData: Timeout waiting for data. Requested: " + to_string(bytesToRead) +  ", Received: " + to_string(bytesRead)); return false; } 
        else { Debug.Log("PollData: Successfully polled " + to_string(bytesRead) + " bytes of data."); return true; }
    }



}