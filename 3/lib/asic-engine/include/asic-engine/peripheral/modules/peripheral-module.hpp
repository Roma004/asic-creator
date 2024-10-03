#pragma once
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/peripheral/abstracts/peripheral.hpp>

class PeripheralModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "peripheral";
        return type;
    };

    /**
     * @brief Создать новый объект типа "Периферийне устройство"
     * @param[gate] входной интерфейс устройства на шине
     * @param[settings] настройки конкретной единицы устройства
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<PeripheralInterface> make_peripheral(
        EndpointInterface &gate, const ModuleSettings &settings
    ) const = 0;
};
