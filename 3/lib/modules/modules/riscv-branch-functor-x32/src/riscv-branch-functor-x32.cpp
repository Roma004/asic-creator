#include "../include/riscv-branch-functor-x32/riscv-branch-functor-x32.hpp"

void BFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    reg_t &rs2 = dynamic_cast<reg_t &>(i.get_rsn(2));
    get_operation(i)(rs1.get_value(), rs2.get_value(), i.get_immediate(), pc);
}

void beq_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = (r1 == r2 ? pc + imm : pc + 4);
}

void bne_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = (r1 != r2 ? pc + imm : pc + 4);
}

void blt_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = ((int32_t)r1 < (int32_t)r2 ? pc + imm : pc + 4);
}

void bltu_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = (r1 < r2 ? pc + imm : pc + 4);
}

void bge_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = ((int32_t)r1 >= (int32_t)r2 ? pc + imm : pc + 4);
}

void bgeu_op(uint32_t r1, uint32_t r2, int32_t imm, std::atomic_uint32_t &pc) {
    pc = (r1 >= r2 ? pc + imm : pc + 4);
}

// clang-format off
const BFunctor::op_map_t BFunctor::ops{
/* beq */ { 0x00063, beq_op },
/* bne */ { 0x00163, bne_op },
/* blt */ { 0x00463, blt_op },
/* bltu*/ { 0x00563, bltu_op},
/* bge */ { 0x00663, bge_op },
/* bgeu*/ { 0x00763, bgeu_op},
};

