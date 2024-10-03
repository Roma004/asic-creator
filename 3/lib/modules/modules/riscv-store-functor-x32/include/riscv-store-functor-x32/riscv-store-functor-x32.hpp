#pragma once
#include <modules-common/data-master-port.hpp>
#include <modules-common/riscv-functor.hpp>

using s_op_signature = void(uint32_t, uint32_t, int32_t, DataMasterPort &);
constexpr const static inline char s_type[] = "S";

class SFunctor : public RiscVFunctor<s_op_signature, s_type> {
    static const std::map<uint32_t, std::function<s_op_signature>> ops;
    DataMasterPort &port;

  public:
    SFunctor(DataMasterPort &port) : RiscVFunctor(ops), port(port) {};
    void handle_instruction(GenericInstruction &i) override;
};
