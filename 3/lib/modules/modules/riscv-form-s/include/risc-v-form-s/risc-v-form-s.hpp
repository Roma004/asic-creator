#pragma once
#include "asic-engine/cpu/register-set.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>

class SForm : public InstructionForm {
  public:
    constexpr static const std::string form = "S";

    SForm(RegistersX32 &blk) : reg_blk(blk) {};
    const std::string &get_form() const noexcept override { return form; };
    RegisterSet get_registers(BaseInstruction i) const override;
    uint32_t get_immediate(BaseInstruction i) const override;
    uint32_t get_minor(BaseInstruction i) const override;
    uint32_t get_sub_minor(BaseInstruction i) const override;

  private:
    struct SDesc {
        uint32_t opcode : 7;
        uint32_t imm4_0 : 5;
        uint32_t funct3 : 3;
        uint32_t rs1    : 5;
        uint32_t rs2    : 5;
        uint32_t imm11_5: 7;

        SDesc(uint32_t v) { *(uint32_t *)this = v; }
        int get_imm() {
            int mask = 1 << 11;
            return ((int32_t)get_uimm() ^ mask) - mask;
        }
        int get_uimm() { return imm11_5 << 5 | imm4_0; }
    };

    RegistersX32 &reg_blk;
};

