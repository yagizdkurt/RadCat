#include "pyManager.hpp"
#include "App.hpp"

#define RG pybind11::gil_scoped_release _gil_release;

void PyManager::connectMiniX(){ RG
    controller->queueTask([this]() {
        controller->dataHandler.connectMiniX();
    });
}

void PyManager::disconnectMiniX(){ RG
    controller->queueTask([this]() {
        controller->dataHandler.disconnectMiniX();
    });
}

void PyManager::testButton(){ RG
    Debug.Log("=== STARTING DEVICE DIAGNOSTICS ===");
    Debug.Log("Set Voltage = " + std::to_string(controller->m_targetMinixVoltage.load()) + " kV");
    Debug.Log("Set Current = " + std::to_string(controller->m_targetMinixCurrent.load()) + " uA");
}

void PyManager::setTargetVoltage(double voltage){ RG
    controller->queueTask([this, voltage]() {
        controller->dataHandler.setVoltage(voltage);
    });
}

void PyManager::setTargetCurrent(double current){ RG
    controller->queueTask([this, current]() {
        controller->dataHandler.setCurrent(current);
    });
}

