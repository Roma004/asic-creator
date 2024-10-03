#include "risc-v-form-s/risc-v-form-s.hpp"

RegisterSet SForm::get_registers(BaseInstruction i) const {
    SDesc d(i.get_instruction());
    auto rs1 = reg_blk.make_reference(d.rs1);
    auto rs2 = reg_blk.make_reference(d.rs2);
    return RegisterSet({rs1, rs2});
}

uint32_t SForm::get_immediate(BaseInstruction i) const {
    return SDesc(i.get_instruction()).get_imm();
}

uint32_t SForm::get_minor(BaseInstruction i) const {
    return SDesc(i.get_instruction()).funct3;
}

uint32_t SForm::get_sub_minor(BaseInstruction i) const { return 0; }

