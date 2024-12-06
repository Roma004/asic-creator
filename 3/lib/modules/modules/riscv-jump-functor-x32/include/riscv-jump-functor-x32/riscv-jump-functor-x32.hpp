#pragma once
#include <modules-common/riscv-functor.hpp>
#include <atomic>

using j_op_signature = uint32_t(int32_t, std::atomic_uint32_t &);
constexpr const static inline char j_type[] = "J";

class JFunctor : public RiscVFunctor<j_op_signature, j_type> {
    static const std::map<uint32_t, std::function<j_op_signature>> ops;
    std::atomic_uint32_t &pc;

  public:
    JFunctor(std::atomic_uint32_t &pc) : RiscVFunctor(ops), pc(pc) {};
    void handle_instruction(GenericInstruction &i) override;
};
