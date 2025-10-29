#pragma once
#include <vector>
#include <memory>
#include "devices/deviceCore.hpp"
#include "ftd2xx.h"


class DeviceHandler {
public:
    std::vector<std::unique_ptr<EmptyDevice>> activeDevices;
    const static bool debug = false;


    int FTDIScan();
    
};