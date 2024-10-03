#pragma once
#include "mod-loader/module-settings.hpp"
#include <asic-engine/cpu/abstracts/control-device.hpp>
#include <asic-engine/cpu/registries/cpu-context.hpp>
#include <mod-loader/module-interface.hpp>

#include <memory>

/**
 * @brief Интерфейс модуля управляющего устройства
 *
 * Объект описывает методы, необходимые для подключения нового типа управляющего
 * устройства. Для подробностей см. ModuleManager
 * */
class ControlDeviceModule : public ModuleInterface {
    constexpr static const std::string type = "control_device";
  public:
    const std::string &get_component_type() const override { return type; };
    /**
     * @brief Создать новый объект типа "Управляющее устройство"
     *
     * Метод должен инициализировать новый объект типа, реализуемого модулем
     *
     * Стоит отметить, что новый объект может существовать более чем в одном
     * экземпляре, так что ему не следует иметь никакой статической логики,
     * если разделение каких-то ресурсов между ВСЕМИ созданными процессорами не
     * предполагается.
     *
     * @param[ctx]
     *   Объект, хранящий все зарегистрированные для данного процессора
     *   компоненты
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<ControlDeviceInterface>
    make_control_device(CPUContext &ctx, ModuleSettings &stt) const = 0;
};
