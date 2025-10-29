#pragma once
#include <string>
#include <tuple>
#include <map>
#include <memory>
#include <functional>
#include <vector>
#include <typeindex>
#include <algorithm>
#include "deviceRegistry.hpp"

// Device registration macro REGISTER_DEVICE(class, "Name");
#define REGISTER_DEVICE(TYPE, NAME) \
    static inline bool registered_##TYPE = [](){ \
        auto tempDevice = std::make_unique<TYPE>(); \
        DeviceRegistry::registry()[NAME] = { [](){ return std::make_unique<TYPE>(); }, \
            BaseDevice<>::tuple_types<decltype(std::declval<TYPE>().components)>::get(), \
            tempDevice->deviceInfo }; \
        return true; \
    }()

// Polymorphic base for all devices
class EmptyDevice {
public:
    virtual ~EmptyDevice() = default;
    virtual void* getComponent(const std::type_info& ti) { return nullptr; }
    DeviceRegistry::RegistryEntry::DeviceInfo deviceInfo;
};

// All devices should inherit from this template
template<typename... Components> class BaseDevice : public EmptyDevice {
public:
    BaseDevice() : components(Components(*this)...) {}
    virtual ~BaseDevice() {}

    // Pure virtual methods to be implemented by derived classes
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual void cycleCheck() = 0;
    virtual bool checkConnectionMatch() = 0;
    virtual double readValue(const std::string& parameter) = 0;
    virtual bool setValue(const std::string& parameter, double value) = 0;

    // Components
    std::tuple<Components...> components;
    std::tuple<Components...>& getComponents() { return components; }
    const std::tuple<Components...>& getComponents() const { return components; }
    template<typename T> T& getComponent() { return std::get<T>(components); }
    template<typename T> const T& getComponent() const { return std::get<T>(components); }
    void componentUpdate() { ([&] { getComponent<Components>().update(); }(), ...); }
    void* getComponent(const std::type_info& ti) override {
        void* result = nullptr;
        ([&] { if (typeid(Components) == ti) result = static_cast<void*>(&getComponent<Components>()); }(), ...);
        return result;
    }

    template<typename Tuple> struct tuple_types; // Helper to extract component type_index list from a tuple type
    template<typename... Ts> struct tuple_types<std::tuple<Ts...>> {
        static std::vector<std::type_index> get() { return { std::type_index(typeid(Ts))... }; }
    };

};

// Specialization for empty component list (backward compatibility)
using BaseDeviceEmpty = BaseDevice<>;