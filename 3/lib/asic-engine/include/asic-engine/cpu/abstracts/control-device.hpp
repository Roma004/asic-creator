#pragma once
#include <atomic>
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
    virtual ~ControlDeviceInterface() = default;
    /** @brief Получить счётчик команд */
    virtual std::atomic_uint32_t &get_pc() noexcept = 0;
    virtual const std::atomic_uint32_t &get_pc() const noexcept = 0;
    virtual bool is_completed() const noexcept = 0;
};
