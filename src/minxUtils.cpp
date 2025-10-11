#include "minxUtils.hpp"
#include "utilities.hpp"
using namespace std;
namespace MinixUtilities {

    void setClockDivisor(unsigned char* tx, int& pos, int clockDivisor) {
        tx[pos++] = CMD_SET_CLK_DIVISOR;
        tx[pos++] = clockDivisor & 0xFF;
        tx[pos++] = (clockDivisor >> 8) & 0xFF;
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
        int tempRaw = (MSB << 4) + (LSB >> 4); double temp_c;
        if (MSB >= 0x80) { temp_c = static_cast<double>(tempRaw - 4096) * 0.0625; } 
        else { temp_c = static_cast<double>(tempRaw) * 0.0625; }
        double temperature = isF ? (temp_c * 9.0 / 5.0 + 32.0) : temp_c;
        // Debug.Log("MSB: " + std::to_string(MSB) + ", LSB: " + std::to_string(LSB) + ", Raw 12-bit: " + std::to_string(tempRaw) + ", Temperature: " + std::to_string(temperature) + (isF ? "°F" : "°C"));
        return temperature;
    }

    void purgeBuffers(FT_HANDLE handle){
        FT_STATUS status = FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
        Utilities::sleepMs(20);
        // Debug.Log("Purged RX and TX buffers.");
    }

    void activateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState) {
        tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
        SET(HighByteHiLowState, TSCS); 
        tx[pos++] = HighByteHiLowState;
        tx[pos++] = OUTPUTMODE_H;
        // Debug.Log("Activated temperature sensor (TSCS low).");
    }

    void deactivateTemperatureSensor(unsigned char* tx, int& pos, unsigned char& HighByteHiLowState) {
        tx[pos++] = CMD_SET_DATA_BITS_HIGHBYTE;
        CLEAR(HighByteHiLowState, TSCS);
        tx[pos++] = HighByteHiLowState;
        tx[pos++] = OUTPUTMODE_H;
        // Debug.Log("Deactivated temperature sensor (TSCS high).");
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
        else { 
            Debug.Log("PollData: Successfully polled " + to_string(bytesRead) + " bytes of data."); 
            return true; 
        }
    }



}