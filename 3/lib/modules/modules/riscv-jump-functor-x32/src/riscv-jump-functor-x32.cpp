#include "../include/riscv-jump-functor-x32/riscv-jump-functor-x32.hpp"

void JFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    rd.set_value(get_operation(i)(i.get_immediate(), pc));
}

// clang-format off

static inline uint32_t jal_op(int32_t imm, std::atomic_uint32_t &pc) {
    uint32_t res = pc + 4;
    pc += (int32_t)imm;
    return res;
}


inline const JFunctor::op_map_t JFunctor::ops{
/* jal */ { 0x0006f, jal_op },
};

