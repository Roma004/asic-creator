#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <mod-loader/module-interface.hpp>

/**
 * @brief Интерфейс модуля ветвителя шины
 *
 * Для подробностей см. ModuleManager
 * */
class SwitchModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "device-gate";
        return type;
    };

    /**
     * @brief Создать новый объект типа "Ветвитель"
     * @param[switch_id]
     *   уникальный идентификатор переключателя, необходимый для построения
     *   обратного маршрута пакета по шине. Подробнее см. SwitchContext
     * @param[default_slave]
     *   Устройство, на которое будут перенаправляться пакеты, не попавшие ни в
     *   один диапазон адрессов.
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<SwitchInterface> make_switch(
        uint8_t switch_id, SlaveInterface &default_slave, ModuleSettings &stt
    ) const = 0;
};
