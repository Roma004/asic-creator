#pragma once
#include "asic-engine/cpu/abstracts/instruction-functor.hpp"
#include "asic-engine/cpu/registries/cpu-context.hpp"
#include "mod-loader/module-settings.hpp"
#include <mod-loader/module-interface.hpp>

#include <memory>

/**
 * @brief Интерфейс модуля функтора инструкции
 *
 * Объект описывает методы, необходимые для подключения к проекту стороннего
 * функтора инструкции. Для подробностей см. ModuleManager
 * */
class InstructionFunctorModule : public ModuleInterface {
  public:
    const std::string &get_component_type() const override {
        static const std::string &type = "instruction-functor";
        return type;
    };
    /**
     * @brief Создать новый объект типа "Функтор инструкции"
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
    virtual std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const = 0;
};

