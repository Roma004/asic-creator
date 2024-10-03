#include "../include/riscv-store-functor-x32/riscv-store-functor-x32.hpp"

void SFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    reg_t &rs2 = dynamic_cast<reg_t &>(i.get_rsn(2));

    get_operation(i)(rs1.get_value(), rs2.get_value(), i.get_immediate(), port);
}

void sw_op(uint32_t r1, uint32_t r2, int32_t imm, DataMasterPort &port) {
    debug.write("SW: {:x},{}({:x})", r2, imm, r1);
    port.write32(r1 + imm, r2);
}

void sh_op(uint32_t r1, uint32_t r2, int32_t imm, DataMasterPort &port) {
    port.write16(r1 + imm, r2);
}

void sb_op(uint32_t r1, uint32_t r2, int32_t imm, DataMasterPort &port) {
    port.write8(r1 + imm, r2);
}

// clang-format off

const SFunctor::op_map_t SFunctor::ops{
/* sw */ {0x00223, sw_op},
/* sh */ {0x00123, sh_op},
/* sb */ {0x00023, sb_op},
};

