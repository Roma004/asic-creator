#include "risc-v-form-b/risc-v-form-b.hpp"

RegisterSet BForm::get_registers(BaseInstruction i) const {
    BDesc d(i.get_instruction());
    auto rs1 = reg_blk.make_reference(d.rs1);
    auto rs2 = reg_blk.make_reference(d.rs2);
    return RegisterSet({rs1, rs2});
}

uint32_t BForm::get_immediate(BaseInstruction i) const {
    return BDesc(i.get_instruction()).get_imm();
}

uint32_t BForm::get_minor(BaseInstruction i) const {
    return BDesc(i.get_instruction()).funct3;
}

uint32_t BForm::get_sub_minor(BaseInstruction i) const { return 0; }

