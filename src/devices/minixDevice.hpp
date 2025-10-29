#pragma once
#include "baseDevice.hpp"
#include "comps/FTDIConnection.hpp"

class MiniXDevice : public BaseDevice<FTDIConnection> {
public:
    static constexpr bool debug = false;
    MiniXDevice() {
        deviceInfo.model = "Mini-X";
    }

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

    // Hardware State Variables
    unsigned char LowByteHiLowState;
    unsigned char HighByteHiLowState;


};