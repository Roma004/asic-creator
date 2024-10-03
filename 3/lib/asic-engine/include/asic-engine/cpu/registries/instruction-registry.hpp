#pragma once

#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <asic-engine/cpu/abstracts/instruction-functor.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <cstdint>
#include <map>

class InstructionRegistry {
  public:
    void add_instruction(
        uint32_t opcode,
        InstructionForm &form,
        InstructionFunctor &funct,
        ExecutorDeviceInterface &exec
    );

    GenericInstruction parse_instruction(BaseInstruction i) const;

    InstructionFunctor &get_functor(BaseInstruction i) const;

    InstructionFunctor &get_functor(const GenericInstruction &i) const;

    ExecutorDeviceInterface &get_executor(BaseInstruction i) const;

  private:
    struct registry_entry {
        const InstructionForm &form;
        InstructionFunctor &funct;
        ExecutorDeviceInterface &exec;
    };

    using registry_t = std::map<uint32_t, registry_entry>;

    registry_t::const_iterator get_by_opcode(uint32_t opcode) const;

    InstructionFunctor &get_functor_by_opcode(uint32_t opcode) const;

    const InstructionForm &get_form_by_opcode(uint32_t opcode) const;

    ExecutorDeviceInterface &get_executor_by_opcode(uint32_t opcode) const;

    registry_t registry;
};
