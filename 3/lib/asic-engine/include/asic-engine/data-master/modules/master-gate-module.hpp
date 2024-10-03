#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <mod-loader/module-interface.hpp>

/**
 * @brief Интерфейс модуля Ведущего устройтсва шины
 *
 * Для подробностей см. ModuleManager
 * */
class MasterGateModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "master-gate";
        return type;
    };

    /**
     * @brief Создать новый объект типа "Ведущее устройство"
     * @param[slv] ведомое устройство, подключаемое к данному
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<RootInterface>
    make_master_gate(SlaveInterface &slv, ModuleSettings &stt) const = 0;
};

