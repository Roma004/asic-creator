#pragma once
#include "asic-engine/cpu/abstracts/registers.hpp"
#include "mod-loader/module-settings.hpp"
#include <mod-loader/module-interface.hpp>

#include <memory>

/**
 * @brief Интерфейс модуля блока регистров
 *
 * Объект описывает методы, необходимые для подключения к проекту стороннего
 * блока регистров. Для подробностей см. ModuleManager
 * */
class RegistersModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "registers";
        return type;
    };
    /**
     * @brief Создать новый объект типа "Блок регистров"
     *
     * Метод должен инициализировать новый объект типа, реализуемого модулем
     *
     * Стоит отметить, что новый объект может существовать более чем в одном
     * экземпляре, так что ему не следует иметь никакой статической логики,
     * если разделение каких-то ресурсов между ВСЕМИ созданными процессорами не
     * предполагается.
     *
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<RegisterBlock> make_registers(ModuleSettings &stt
    ) const = 0;
};
