#include "risc-v-form-j/risc-v-form-j.hpp"

RegisterSet JForm::get_registers(BaseInstruction i) const {
    JDesc d(i.get_instruction());
    auto rd = reg_blk.make_reference(d.rd);
    return RegisterSet(rd);
}

uint32_t JForm::get_immediate(BaseInstruction i) const {
    return JDesc(i.get_instruction()).get_imm();
}

uint32_t JForm::get_minor(BaseInstruction i) const { return 0; }

uint32_t JForm::get_sub_minor(BaseInstruction i) const { return 0; }
