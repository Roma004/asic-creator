#pragma once
#include <modules-common/riscv-functor.hpp>
#include <atomic>

using u_op_signature = uint32_t(int32_t, std::atomic_uint32_t &);
constexpr const static inline char u_type[] = "U";

class UFunctor : public RiscVFunctor<u_op_signature, u_type> {
    static const std::map<uint32_t, std::function<u_op_signature>> ops;
    std::atomic_uint32_t &pc;

  public:
    UFunctor(std::atomic_uint32_t &pc) : RiscVFunctor(ops), pc(pc) {};
    void handle_instruction(GenericInstruction &i) override;
};
