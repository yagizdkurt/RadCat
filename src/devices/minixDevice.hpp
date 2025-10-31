#pragma once
#include "deviceCore.hpp"
#include "comps/FTDIConnection.hpp"

class MiniXDevice : public BaseDevice<FTDIConnection> {
public:
    static constexpr bool debug = false;
    MiniXDevice() { startingParameters(); setupTasks(); }
    std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
    static inline const DeviceRegistry::RegistryEntry::DeviceInfo deviceInfo = {"Mini-X","1.0","Manufacturer","Description"};

    // Implement virtual methods
    virtual bool connect() override;
    virtual bool disconnect() override;
    virtual void update() override;
    virtual void setupTasks() override;
    virtual double readValue(const std::string& parameter) override;
    virtual bool setValue(const std::string& parameter, double value) override;

    // Additional MiniX-specific methods can be added here
    bool initialize();
    void setVoltage(double voltage);
    void setCurrent(double voltage);
    void setHVOnOff(bool on);
    bool initializeMiniX();
    void testread();


    // Hardware State Variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;

private:
    bool initializeGPIOs();
    void startingParameters();
    double readVoltage();
    double readCurrent();
    double readTemperature();
    bool safetyChecks();
    bool setupTemperatureSensor();
    bool setupClockDivisor();

    // Minix Components
    FTDIConnection& connection = getComponent<FTDIConnection>();

    // Minix-specific variables
    bool hvOn = false;
    bool HVOffErr = false;
    bool HVSetErr = false;
    bool CurrentSetErr = false;

    // Minix Status Variables
    double currentVoltage;
    double currentCurrent;
    double currentTemperature;

    // Mini-X Configuration Parameters
    double DefaultHighVoltage;
    double HighVoltageMin;
    double HighVoltageMax;
    double TemperatureMax;
    double TemperatureMin;
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


};