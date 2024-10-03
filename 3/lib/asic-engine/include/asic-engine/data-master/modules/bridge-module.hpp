#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <mod-loader/module-interface.hpp>

/**
 * @brief Интерфейс модуля моста шины
 *
 * Для подробностей см. ModuleManager
 * */
class BridgeModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "bridge";
        return type;
    };

    /**
     * @brief Создать новый объект типа "Мост"
     * @param[slv] Устройство, на которое будут передаваться пакеты
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<BridgeInterface>
    make_bridge(SlaveInterface &slv, ModuleSettings &stt) const = 0;
};
