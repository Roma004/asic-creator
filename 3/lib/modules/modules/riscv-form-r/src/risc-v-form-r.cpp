#include "risc-v-form-r/risc-v-form-r.hpp"

RegisterSet RForm::get_registers(BaseInstruction i) const {
    RDesc d(i.get_instruction());
    auto rd = reg_blk.make_reference(d.rd);
    auto rs1 = reg_blk.make_reference(d.rs1);
    auto rs2 = reg_blk.make_reference(d.rs2);
    return RegisterSet(rd, {rs1, rs2});
}

uint32_t RForm::get_immediate(BaseInstruction) const { return 0; }

uint32_t RForm::get_minor(BaseInstruction i) const {
    return RDesc(i.get_instruction()).funct3;
}

uint32_t RForm::get_sub_minor(BaseInstruction i) const {
    return RDesc(i.get_instruction()).funct7;
}
