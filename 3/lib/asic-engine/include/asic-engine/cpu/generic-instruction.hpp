#pragma once
#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <asic-engine/cpu/register-set.hpp>
#include <asic-engine/exceptions.hpp>

/**
 * @brief Класс обобщённой инструкции
 *
 * Класс предоставляет доступ ко всем параметрам инструкции, необходимым для её
 * исполнения:
 * * код операции (основной, дополнительный и расширенный)
 * * формат инструкции
 * * запрос на владение регистрами
 * * набор регистров (операнды и результат)
 * */
class GenericInstruction {
    using register_t = RegisterBlock::RegisterReference;

  public:
    GenericInstruction(
        const InstructionForm &form, BaseInstruction instruction
    );
    GenericInstruction(const GenericInstruction &) = delete;
    GenericInstruction(GenericInstruction &&);

    /** @brief Получить обект формата запроса */
    const InstructionForm &get_form() const noexcept;

    /** @brief Получить обект базовой инструкции */
    BaseInstruction get_instruction() const noexcept;

    /**
     * @brief Получить регистр результата операции
     * @return объект ссылки на регистр результата
     * @throw
     *   GenericInstructionExc если регистр результата не предполагается
     *   для данного формата опреации
     * */
    register_t &get_rd();

    /**
     * @brief Получить регистр-операнд под номером \p n
     * @param[n] номер регистра опреанда для получения
     * @return объект ссылки на регистр операнда
     * @throw
     *   GenericInstructionExc если регистр результата не предполагается
     *   для данного формата опреации
     * */
    register_t &get_rsn(size_t n);

    /** @brief Получить мгнвенную константу заданную в инструкции */
    uint32_t get_immediate() const noexcept;

    /** @brief Получить основной код опреации */
    uint32_t get_opcode() const noexcept;

    /** @brief Получить дополнительный код опреации */
    uint32_t get_minor() const noexcept;

    /** @brief Получить расширенный код опреации */
    uint32_t get_sub_minor() const noexcept;

    /** @brief Пролучить объект запроса на владение регистрами */
    RegisterRequest &get_request() noexcept;

    /** @brief статус готовности инструкции к исполнению */
    constexpr bool is_ready() const noexcept;

  private:
    const InstructionForm &form;
    BaseInstruction instr;
    RegisterSet regs;
    RegisterRequest req;
};

constexpr bool GenericInstruction::is_ready() const noexcept {
    return req.is_ready();
}
