#pragma once
#include <asic-engine/handler-device.hpp>

/* @brief Интерфейс периферийного устройства */
class PeripheralInterface : public HandlerDevice {
public:
    virtual ~PeripheralInterface() = default;
};
