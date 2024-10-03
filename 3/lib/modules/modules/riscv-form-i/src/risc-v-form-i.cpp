#include "risc-v-form-i/risc-v-form-i.hpp"

RegisterSet IForm::get_registers(BaseInstruction i) const {
    IDesc d(i.get_instruction());
    auto rd = reg_blk.make_reference(d.rd);
    auto rs1 = reg_blk.make_reference(d.rs1);
    return RegisterSet(rd, {rs1});
}

uint32_t IForm::get_immediate(BaseInstruction i) const {
    switch (i.get_opcode()) {
    case 0x13: // ariphmetic with immediates
        switch (get_minor(i)) {
        case 0x1:
        case 0x5: return IDesc1(i.get_instruction()).get_uimm();
        default: return IDesc(i.get_instruction()).get_imm();
        }
    case 0x03: // load instructions
    case 0x67: // jalr
    default: return IDesc(i.get_instruction()).get_imm(); ;
    }
}

uint32_t IForm::get_minor(BaseInstruction i) const {
    return IDesc(i.get_instruction()).funct3;
}

uint32_t IForm::get_sub_minor(BaseInstruction i) const {
    switch (i.get_opcode()) {
    case 0x13: // ariphmetic with immediates
        switch (get_minor(i)) {
        case 0x1:
        case 0x5: return IDesc1(i.get_instruction()).funct7;
        default: return 0;
        }
    case 0x03: // load instructions
    case 0x67: // jalr
    default: return 0;
    }
}
