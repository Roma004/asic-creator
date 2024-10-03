#include "risc-v-form-u/risc-v-form-u.hpp"

RegisterSet UForm::get_registers(BaseInstruction i) const {
    UDesc d(i.get_instruction());
    auto rd = reg_blk.make_reference(d.rd);
    return RegisterSet(rd);
}

uint32_t UForm::get_immediate(BaseInstruction i) const {
    return UDesc(i.get_instruction()).get_imm();
}

uint32_t UForm::get_minor(BaseInstruction i) const { return 0; }

uint32_t UForm::get_sub_minor(BaseInstruction i) const { return 0; }

