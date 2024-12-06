#pragma once
#include <modules-common/riscv-functor.hpp>
#include <atomic>

using jalr_op_signature = uint32_t(uint32_t, int32_t, std::atomic_uint32_t &);
constexpr const static inline char jalr_type[] = "I";

class JALRFunctor : public RiscVFunctor<jalr_op_signature, jalr_type> {
    static const std::map<uint32_t, std::function<jalr_op_signature>> ops;
    std::atomic_uint32_t &pc;

  public:
    JALRFunctor(std::atomic_uint32_t &pc) : RiscVFunctor(ops), pc(pc) {};
    void handle_instruction(GenericInstruction &i) override;
};
