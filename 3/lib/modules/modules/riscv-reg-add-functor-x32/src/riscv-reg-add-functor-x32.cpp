#include "../include/riscv-reg-add-functor-x32/riscv-reg-add-functor-x32.hpp"

void RFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    reg_t &rs2 = dynamic_cast<reg_t &>(i.get_rsn(2));
    rd.set_value(get_operation(i)(rs1.get_value(), rs2.get_value()));
}

// clang-format off
const RFunctor::op_map_t RFunctor::ops{
/* and */ { 0x00733, [](uint32_t r1, uint32_t r2) { return r1 & r2;} },
/* or  */ { 0x00633, [](uint32_t r1, uint32_t r2) { return r1 | r2;} },
/* sra */ { 0x20533, [](uint32_t r1, uint32_t r2) { return  (int32_t)r1 >> (r2 & 0b11111);} },
/* srl */ { 0x00533, [](uint32_t r1, uint32_t r2) { return (uint32_t)r1 >> (r2 & 0b11111);} },
/* xor */ { 0x00433, [](uint32_t r1, uint32_t r2) { return r1 ^ r2;} },
/* sll */ { 0x00133, [](uint32_t r1, uint32_t r2) { return r1 << (r2 & 0b11111);} },
/* add */ { 0x00033, [](uint32_t r1, uint32_t r2) { return r1 + r2;} },
/* sub */ { 0x20033, [](uint32_t r1, uint32_t r2) { return r1 - r2;} },
/* sltu*/ { 0x00333, [](uint32_t r1, uint32_t r2) { return r1 < r2 ? 1 : 0;} },
/* slt */ { 0x00233, [](uint32_t r1, uint32_t r2) {
                    return (int32_t)r1 < (int32_t)r2 ? 1 : 0;} },
};
