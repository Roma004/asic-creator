#pragma once
#include "asic-engine/cpu/abstracts/data-master-port.hpp"
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-settings.hpp"
#include <mod-loader/module-interface.hpp>

#include <memory>

/**
 * @brief Интерфейс модуля порта внешней шины
 *
 * Объект описывает методы, необходимые для подключения к проекту стороннего
 * порта взаимодействия с внешней шиной. Для подробностей см. ModuleManager
 * */
class DataMasterPortModule : public ModuleInterface {
    constexpr static const char type_str[] = "data_master_port";

  public:
    const std::string &get_component_type() const override {
        static const std::string &type = type_str;
        return type;
    };
    /**
     * @brief Создать новый объект типа "Порт внешней шины"
     *
     * Метод должен инициализировать новый объект типа, реализуемого модулем
     *
     * Стоит отметить, что новый объект может существовать более чем в одном
     * экземпляре, так что ему не следует иметь никакой статической логики,
     * если разделение каких-то ресурсов между ВСЕМИ созданными процессорами не
     * предполагается.
     *
     * @param[gate] Вход внешней шины, к которому порт подключается
     * @param[stt] настройки модуля
     * @return указатель на новосозданный объект
     * */
    virtual std::unique_ptr<DataMasterPortInterface>
    make_port(RootInterface &gate, ModuleSettings &stt) const = 0;
};
