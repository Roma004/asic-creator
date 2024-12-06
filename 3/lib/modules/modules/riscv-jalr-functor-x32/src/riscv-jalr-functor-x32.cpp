#include "../include/riscv-jalr-functor-x32/riscv-jalr-functor-x32.hpp"

void JALRFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    rd.set_value(get_operation(i)(rs1.get_value(), i.get_immediate(), pc));
}

uint32_t jalr_op(uint32_t r1, int32_t imm, std::atomic_uint32_t &pc) {
    uint32_t res = pc + 4;
    pc = r1 + imm;
    return res;
}

// clang-format off
const JALRFunctor::op_map_t JALRFunctor::ops{
/* jalr */ { 0x00067, jalr_op },
};

