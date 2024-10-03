#include "../include/riscv-load-functor-x32/riscv-load-functor-x32.hpp"

void LFunctor::handle_instruction(GenericInstruction &i) {
    reg_t &rd = dynamic_cast<reg_t &>(i.get_rd());
    reg_t &rs1 = dynamic_cast<reg_t &>(i.get_rsn(1));
    rd.set_value(get_operation(i)(rs1.get_value(), i.get_immediate(), port));
}

uint32_t lw_op(uint32_t r1, int32_t imm, DataMasterPort &port) {
    debug.write("LW: {}({:x})", imm, r1);
    return port.read32(r1 + imm);
}

uint32_t lh_op(uint32_t r1, int32_t imm, DataMasterPort &port) {
    int16_t res = port.read16(r1 + imm);
    return (uint32_t)res;
}

uint32_t lhu_op(uint32_t r1, int32_t imm, DataMasterPort &port) {
    uint16_t res = port.read16(r1 + imm);
    return (uint32_t)res;
}

uint32_t lb_op(uint32_t r1, int32_t imm, DataMasterPort &port) {
    int8_t res = port.read8(r1 + imm);
    return (uint32_t)res;
}

uint32_t lbu_op(uint32_t r1, int32_t imm, DataMasterPort &port) {
    uint8_t res = port.read8(r1 + imm);
    return (uint32_t)res;
}

// clang-format off
const LFunctor::op_map_t LFunctor::ops{
/* lw */ { 0x00203, lw_op },
/* lh */ { 0x00103, lh_op },
/* lhu*/ { 0x00503, lhu_op },
/* lb */ { 0x00003, lb_op },
/* lbu*/ { 0x00403, lbu_op },
};

