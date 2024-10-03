#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <mod-loader/module-interface.hpp>

/**
 * @brief Интерфейс модуля обработчика неверных пакетов
 *
 * Для подробностей см. ModuleManager
 * */
class DefaultDeviceModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "bridge";
        return type;
    };

    /**
     * @brief Создать новый объект
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<DefaultDeviceInterface>
    make_default_device(ModuleSettings &stt) const = 0;
};
