#pragma once
#include "ftd2xx.h"
#include "minixc.hpp"
#include "utilities.hpp"
#include "minxUtils.hpp"
#include "debug.hpp"

class DIOHandler;
class Controller;

//Serial number big= 1118880
class MinixController {
public:
    DIOHandler* DataHandler;
    Controller* controller;
    MinixController() : DataHandler(nullptr), controller(nullptr) {}
    ~MinixController() {}

    // Debugging
    static constexpr bool minixDebug = false;

    // Interface Methods
    bool connectMiniX();
    bool disconnectMiniX();
    double readVoltage();
    double readCurrent();
    double readTemperature();
    bool safetyChecks();
    void setVoltage(double voltage);
    void setCurrent(double voltage);
    void setHVOnOff(bool on);

private:
    // Minix Setup and close Methods
    bool findMinixDevice();
    int getConnectedDevices();
    bool openDevice(const char* serialNumber);
    bool openMPSSE();
    bool initializeMiniX();
    bool closeDevice();
    bool setupTemperatureSensor();
    bool setupClockDivisor();
    void purgeMinixInfo();
    void SetParametersToDefault();

    // Minix Checks
    bool connectionChecks();

    // Minix Control Methods
    void testread();

    //Global Data Variables
    //FTDI D2XX
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    DWORD bytesWritten;
    DWORD bytesRead;

    // Mini-X Device Information Storage
    char minixSerialNumber[16];      // Store serial number (stable identifier)
    char minixDescription[64];       // Store device description
    DWORD minixLocationId;           // Store location ID (USB port)
    DWORD minixDeviceId;             // Store device/product ID
    bool isDeviceOpen = false;
    bool isMpsseOn = false;

    bool hvOn = false;
    bool HVOffErr = false;
    bool HVSetErr = false;
    bool CurrentSetErr = false;

    // Mini-X Configuration Parameters
    double DefaultHighVoltage;
    double HighVoltageMin;
    double HighVoltageMax;
    double HighVoltageConversionFactor;
    double DefaultCurrent;
    double CurrentMin;
    double CurrentMax;
    double CurrentConversionFactor; // controller to tube current conversion factor
    double VRef;					// DAC/ADC reference voltage 
    double DAC_ADC_Scale;			// DAC/ADC scale = 2^resolution bits wide
    double WattageMax;
    double SafetyMargin;
    double SafeWattageMW;

    FT_HANDLE m_ftHandle;

    bool debug = false;
    float result = -1;;
    double voltagekV = 0.;
    double currentuA = 0.;

    // Hardware state variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;

};
