#include "asic-engine/cpu/abstracts/executor-device.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <asic-engine/cpu/abstracts/instruction-functor.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/exceptions.hpp>
#include <cstdint>
#include <format>
#include <map>

void InstructionRegistry::add_instruction(
    uint32_t opcode,
    InstructionForm &form,
    InstructionFunctor &funct,
    ExecutorDeviceInterface &exec
) {
    if (registry.find(opcode) != registry.end())
        throw KeyError(std::format(
            "Instruction with opcode {} is already registered", opcode
        ));
    registry.insert({opcode, registry_entry(form, funct, exec)});
}

GenericInstruction InstructionRegistry::parse_instruction(BaseInstruction i
) const {
    return GenericInstruction(get_form_by_opcode(i.get_opcode()), i);
}

InstructionFunctor &InstructionRegistry::get_functor(BaseInstruction i) const {
    return get_functor_by_opcode(i.get_opcode());
}

InstructionFunctor &InstructionRegistry::get_functor(const GenericInstruction &i
) const {
    return get_functor_by_opcode(i.get_opcode());
}

ExecutorDeviceInterface &InstructionRegistry::get_executor(BaseInstruction i
) const {
    return get_executor_by_opcode(i.get_opcode());
}

InstructionRegistry::registry_t::const_iterator

InstructionRegistry::get_by_opcode(uint32_t opcode) const {
    auto it = registry.find(opcode);
    if (it == registry.end())
        throw KeyError(
            std::format("Instruction with opcode {} is not registered", opcode)
        );
    return it;
}

InstructionFunctor &InstructionRegistry::get_functor_by_opcode(uint32_t opcode
) const {
    return get_by_opcode(opcode)->second.funct;
}

const InstructionForm &InstructionRegistry::get_form_by_opcode(uint32_t opcode
) const {
    return get_by_opcode(opcode)->second.form;
}

ExecutorDeviceInterface &
InstructionRegistry::get_executor_by_opcode(uint32_t opcode) const {
    return get_by_opcode(opcode)->second.exec;
}
