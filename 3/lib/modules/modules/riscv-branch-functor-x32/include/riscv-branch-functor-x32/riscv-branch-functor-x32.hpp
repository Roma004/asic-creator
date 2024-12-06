#pragma once
#include <modules-common/riscv-functor.hpp>
#include <atomic>

using b_op_signature = void(uint32_t, uint32_t, int32_t, std::atomic_uint32_t &);
constexpr const static inline char b_type[] = "B";

class BFunctor : public RiscVFunctor<b_op_signature, b_type> {
    static const std::map<uint32_t, std::function<b_op_signature>> ops;
    std::atomic_uint32_t &pc;

  public:
    BFunctor(std::atomic_uint32_t &pc) : RiscVFunctor(ops), pc(pc) {};
    void handle_instruction(GenericInstruction &i) override;
};
