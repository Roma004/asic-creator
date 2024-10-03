#pragma once
#include "asic-engine/cpu/register-set.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>

class RForm : public InstructionForm {
  public:
    constexpr static const std::string form = "R";

    RForm(RegistersX32 &blk) : reg_blk(blk) {};
    const std::string &get_form() const noexcept override { return form; };
    RegisterSet get_registers(BaseInstruction i) const override;
    uint32_t get_immediate(BaseInstruction) const override;
    uint32_t get_minor(BaseInstruction i) const override;
    uint32_t get_sub_minor(BaseInstruction i) const override;

  private:
    struct RDesc {
        uint32_t opcode: 7;
        uint32_t rd    : 5;
        uint32_t funct3: 3;
        uint32_t rs1   : 5;
        uint32_t rs2   : 5;
        uint32_t funct7: 7;

        RDesc(uint32_t v) { *(uint32_t *)this = v; }
    };

    RegistersX32 &reg_blk;
};
