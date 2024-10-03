#pragma once
#include "asic-engine/cpu/register-set.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>

class BForm : public InstructionForm {
  public:
    constexpr static const std::string form = "B";

    BForm(RegistersX32 &blk) : reg_blk(blk) {};
    const std::string &get_form() const noexcept override { return form; };
    RegisterSet get_registers(BaseInstruction i) const override;
    uint32_t get_immediate(BaseInstruction i) const override;
    uint32_t get_minor(BaseInstruction i) const override;
    uint32_t get_sub_minor(BaseInstruction i) const override;

  private:
    struct BDesc {
        uint32_t opcode : 7;
        uint32_t imm11  : 1;
        uint32_t imm4_1 : 4;
        uint32_t funct3 : 3;
        uint32_t rs1    : 5;
        uint32_t rs2    : 5;
        uint32_t imm10_5: 6;
        uint32_t imm12  : 1;

        BDesc(uint32_t v) { *(uint32_t *)this = v; }
        int32_t get_imm() {
            int mask = 1 << 12;
            return (int32_t)(get_uimm() ^ mask) - mask;
        }
        uint32_t get_uimm() {
            return imm4_1 << 1 | imm10_5 << 5 | imm11 << 11 | imm12 << 12;
        }
    };

    RegistersX32 &reg_blk;
};
