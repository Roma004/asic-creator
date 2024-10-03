#pragma once
#include "asic-engine/cpu/register-set.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>

class UForm : public InstructionForm {
  public:
    constexpr static const std::string form = "U";

    UForm(RegistersX32 &blk) : reg_blk(blk) {};
    const std::string &get_form() const noexcept override { return form; };
    RegisterSet get_registers(BaseInstruction i) const override;
    uint32_t get_immediate(BaseInstruction i) const override;
    uint32_t get_minor(BaseInstruction i) const override;
    uint32_t get_sub_minor(BaseInstruction i) const override;

  private:
    struct UDesc {
        uint32_t opcode: 7;
        uint32_t rd    : 5;
        uint32_t imm20 : 20;

        UDesc(uint32_t v) { *(uint32_t *)this = v; }
        int32_t get_imm() { return imm20; }
    };

    RegistersX32 &reg_blk;
};

