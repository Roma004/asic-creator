#include "../include/riscv-lui-auipc-functor-x32/riscv-lui-auipc-functor-x32.hpp"

void UFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    rd.set_value(get_operation(i)(i.get_immediate(), pc));
}

// clang-format off

inline const UFunctor::op_map_t UFunctor::ops{
/* lui   */ { 0x00037, [](int32_t imm, uint32_t &pc) { return imm << 12;} },
/* auipc */ { 0x00017, [](int32_t imm, uint32_t &pc) { return pc + (imm << 12); } },
};

