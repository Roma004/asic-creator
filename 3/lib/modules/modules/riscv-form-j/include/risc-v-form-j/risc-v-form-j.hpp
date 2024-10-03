#pragma once
#include "asic-engine/cpu/register-set.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>

class JForm : public InstructionForm {
  public:
    constexpr static const std::string form = "J";

    JForm(RegistersX32 &blk) : reg_blk(blk) {};
    const std::string &get_form() const noexcept override { return form; };
    RegisterSet get_registers(BaseInstruction i) const override;
    uint32_t get_immediate(BaseInstruction i) const override;
    uint32_t get_minor(BaseInstruction i) const override;
    uint32_t get_sub_minor(BaseInstruction i) const override;

  private:
    struct JDesc {
        uint32_t opcode  : 7;
        uint32_t rd      : 5;
        uint32_t imm19_12: 8;
        uint32_t imm11   : 1;
        uint32_t imm10_1 : 10;
        uint32_t imm20   : 1;

        JDesc(uint32_t v) { *(uint32_t *)this = v; }
        int get_imm() {
            int mask = 1 << 20;
            return ((int32_t)get_uimm() ^ mask) - mask;
        }
        int32_t get_uimm() {
            return imm10_1 << 1 | imm11 << 11 | imm19_12 << 12 | imm20 << 20;
        }
    };

    RegistersX32 &reg_blk;
};
