#pragma once
#include <modules-common/riscv-functor.hpp>

using i_op_signature = uint32_t(uint32_t, int32_t);
constexpr const static inline char i_type[] = "I";

class IFunctor : public RiscVFunctor<i_op_signature, i_type> {
    static const std::map<uint32_t, std::function<i_op_signature>> ops;

  public:
    IFunctor() : RiscVFunctor(ops) {};
    void handle_instruction(GenericInstruction &i) override;
};
