#pragma once
#include "deviceCore.hpp"
#include "comps/FTDIConnection.hpp"

class MiniXDevice : public BaseDevice<FTDIConnection> {
public:
    static constexpr bool debug = false;
    MiniXDevice() {}
    static inline const DeviceRegistry::RegistryEntry::DeviceInfo deviceInfo = {"Mini-X","1.0","Manufacturer","Description"};

    // Implement virtual methods
    virtual bool connect() override;
    virtual bool disconnect() override;
    virtual void cycleCheck() override;
    virtual bool checkConnectionMatch() override;
    virtual double readValue(const std::string& parameter) override;
    virtual bool setValue(const std::string& parameter, double value) override;

    // Minix Components
    FTDIConnection& connection = getComponent<FTDIConnection>();

    // Additional MiniX-specific methods can be added here
    bool initializeGPIOs();

    // Minix-specific variables
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


    // Hardware State Variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;


};