#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <mod-loader/module-interface.hpp>

/**
 * @brief Интерфейс модуля ведомого устройства шины
 *
 * Для подробностей см. ModuleManager
 * */
class DeviceGateModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "device-gate";
        return type;
    };

    /**
     * @brief Создать новый объект типа "Ведомое устройство"
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<EndpointInterface>
    make_device_gate(ModuleSettings &stt) const = 0;
};
