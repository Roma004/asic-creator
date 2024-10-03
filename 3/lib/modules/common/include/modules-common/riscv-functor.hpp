#pragma once
#include "asic-engine/cpu/generic-instruction.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/abstracts/instruction-functor.hpp>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <stdexcept>

template <typename Signature, const char *FType>
class RiscVFunctor : public InstructionFunctor {
  protected:
    using reg_t = RegistersX32::RegisterReference;
    using op_func = std::function<Signature>;
    using op_map_t = std::map<uint32_t, std::function<Signature>>;

    const op_map_t &operations;

    constexpr uint32_t
    get_key(uint32_t opcode, uint32_t funct3, uint32_t funct7) const noexcept {
        return opcode | funct3 << 8 | funct7 << 12;
    }

    op_func get_operation(GenericInstruction &i) const {
        auto it = operations.find(
            get_key(i.get_opcode(), i.get_minor(), i.get_sub_minor())
        );
        if (it == operations.end())
            throw std::runtime_error(std::format(
                "Unknown instruction `{:x}-{:x}-{:x}`",
                i.get_opcode(),
                i.get_minor(),
                i.get_sub_minor()
            ));
        return it->second;
    }

  public:
    RiscVFunctor(const op_map_t &ops) : operations(ops) {}
    void operator()(GenericInstruction &i) override {
        if (i.get_form().get_form() != FType) {
            throw std::logic_error(std::format(
                "Instruction `{:x}-{:x}-{:x}` with invalid form `{}` passed to "
                "Functor accepting only {}-formed instructions",
                i.get_opcode(),
                i.get_minor(),
                i.get_sub_minor(),
                i.get_form().get_form(),
                FType
            ));
        }
        handle_instruction(i);
    }

    virtual void handle_instruction(GenericInstruction &i) = 0;
};


