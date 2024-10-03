#pragma once

#include <cstdint>

/**
 * @brief
 *     класс предоставляет возможность получить значение кода операции
 *     для инструкции в "сыром виде"
 **/
class BaseInstruction {
  public:
    BaseInstruction(uint32_t inst_word);
    /** @brief получить код опреации */
    constexpr uint32_t get_opcode() const noexcept;
    /** @brief получить инструкцию целиком */
    constexpr uint32_t get_instruction() const noexcept;

  private:
    uint32_t word;
};

constexpr uint32_t BaseInstruction::get_opcode() const noexcept {
    return word & 0x7f;
}

/** @brief получить инструкцию целиком */
constexpr uint32_t BaseInstruction::get_instruction() const noexcept {
    return word;
}
