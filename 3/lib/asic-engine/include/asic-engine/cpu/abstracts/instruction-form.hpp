#pragma once

#include <asic-engine/cpu/abstracts/registers.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/register-set.hpp>
#include <string>

/**
 * @brief Класс задаёт интерфейс формы инструкции.
 *
 * Под формой подразумевается формат операции (какие биты инструкции за какие
 * параметры отвечают).
 * Инструкция может содержать такие примитивы:
 * * регистр результата (rd)
 * * регистры[1..n] опреандов (rs)
 * * мгновенная константа (immediade)
 * * дополнение кода операции (minor)
 * * расширенное дополнение кода опреации (sub_minor)
 *
 * На формат и содержание команды есть всего 2 ограничения:
 * * все команды занимают фиксированный размер (32 бита)
 * * биты[6:0] всегда обозначают основной код операции.
 *
 * В остальном, количество, расположение и разрядность полей определяется
 * архитектором
 *
 * Каждая форма команды дожна иметь строковое название для возможности её
 * идентификации.
 *
 * Ситуация, когда две формы инструкции называются одинаково валидна до тех пор,
 * пока зарегистрированной является только одна из них.
 **/
class InstructionForm {
  public:
    virtual ~InstructionForm() = default;
    /** @brief получить строковое название формы */
    virtual const std::string &get_form() const noexcept = 0;

    /**
     * @brief Получить набор регистров, необходиымх для данной операции
     * @return Объект RegistersSet управляющий работой с регистрами
     * */
    virtual RegisterSet get_registers(BaseInstruction) const = 0;

    /** @brief Получить мгновенную константу, содержащуюся в инструкции */
    virtual uint32_t get_immediate(BaseInstruction) const = 0;

    /** @brief Получить дополнительный код опреации */
    virtual uint32_t get_minor(BaseInstruction) const = 0;

    /** @brief Получить расширение дополнительного кода опреации */
    virtual uint32_t get_sub_minor(BaseInstruction) const = 0;

    /** @brief Получить основной код опреации (совпадает с i.get_opcode()) */
    uint32_t get_opcode(BaseInstruction i) const noexcept {
        return i.get_opcode();
    }
};
