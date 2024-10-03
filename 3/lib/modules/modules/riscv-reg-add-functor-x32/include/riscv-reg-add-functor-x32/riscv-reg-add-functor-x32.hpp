#pragma once
#include <modules-common/riscv-functor.hpp>

using r_op_signature = uint32_t(uint32_t, uint32_t);
constexpr const static inline char r_type[] = "R";

class RFunctor : public RiscVFunctor<r_op_signature, r_type> {
    static const std::map<uint32_t, std::function<r_op_signature>> ops;

  public:
    RFunctor() : RiscVFunctor(ops) {};
    void handle_instruction(GenericInstruction &i) override;
};
