#pragma once
#include <modules-common/riscv-functor.hpp>
#include <modules-common/data-master-port.hpp>

using load_op_signature = uint32_t(uint32_t, int32_t, DataMasterPort &);
constexpr const static inline char load_type[] = "I";

class LFunctor : public RiscVFunctor<load_op_signature, load_type> {
    static const std::map<uint32_t, std::function<load_op_signature>> ops;
    DataMasterPort &port;

  public:
    LFunctor(DataMasterPort &port) : RiscVFunctor(ops), port(port) {};
    void handle_instruction(GenericInstruction &i) override;
};
