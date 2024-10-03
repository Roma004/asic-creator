#pragma once
#include <cstdint>
#include <asic-engine/handler-device.hpp>

/**
 * @brief Интерфейс управляющего устройства
 *
 * Класс описывает интерфейс управляющего устройства.
 *
 * Управляющее устройство в своём методе handle должно загрузить из памяти
 * инструкцию, соответствующую значению счётчика команд, и передать на
 * исполнение одному из исполняющх устройств.
 * */
class ControlDeviceInterface : public HandlerDevice {
  public:
    virtual ~ControlDeviceInterface() {};
    /** @brief Получить счётчик команд */
    virtual uint32_t &get_pc() noexcept = 0;
    virtual const uint32_t &get_pc() const noexcept = 0;
    virtual bool is_completed() const noexcept = 0;
};
