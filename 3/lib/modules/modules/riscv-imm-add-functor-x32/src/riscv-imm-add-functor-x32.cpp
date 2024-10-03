#include "../include/riscv-imm-add-functor-x32/riscv-imm-add-functor-x32.hpp"

void IFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    rd.set_value(get_operation(i)(rs1.get_value(), i.get_immediate()));
}

// clang-format off

const IFunctor::op_map_t IFunctor::ops{
/* andi */ { 0x00713, [](uint32_t r1, int32_t imm) { return r1 & imm;} },
/* ori  */ { 0x00613, [](uint32_t r1, int32_t imm) { return r1 | imm;} },
/* xori */ { 0x00413, [](uint32_t r1, int32_t imm) { return r1 ^ imm;} },
/* sltiu*/ { 0x00313, [](uint32_t r1, int32_t imm) { return r1 < (uint32_t)imm ? 1 : 0;} },
/* slti */ { 0x00213, [](uint32_t r1, int32_t imm) { return (int32_t)r1 < imm ? 1 : 0;} },
/* addi */ { 0x00013, [](uint32_t r1, int32_t imm) { return r1 + imm;} },
/* srai */ { 0x20513, [](uint32_t r1, uint32_t imm) { return  (int32_t)r1 >> imm;} },
/* srli */ { 0x00513, [](uint32_t r1, uint32_t imm) { return (uint32_t)r1 >> imm;} },
/* slli */ { 0x00113, [](uint32_t r1, uint32_t imm) { return r1 << imm;} },
};
