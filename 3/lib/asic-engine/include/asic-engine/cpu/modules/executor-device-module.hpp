#pragma once
#include "asic-engine/cpu/abstracts/executor-device.hpp"
#include "asic-engine/cpu/registries/cpu-context.hpp"
#include <mod-loader/module-interface.hpp>
#include <mod-loader/module-settings.hpp>

#include <memory>

/**
 * @brief Интерфейс модуля исполняющего устройства
 *
 * Объект описывает методы, необходимые для подключения к проекту стороннего
 * исполняющего устройства. Для подробностей см. ModuleManager
 * */
class ExecutorDeviceModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "executor_device";
        return type;
    };
    /**
     * @brief Создать новый объект типа "Исполнящее устройство"
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
    virtual std::unique_ptr<ExecutorDeviceInterface>
    make_executor_device(CPUContext &ctx, ModuleSettings &stt) const = 0;
};
