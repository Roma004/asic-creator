#pragma once

#include "asic-engine/cpu/abstracts/instruction-functor.hpp"
#include "asic-engine/cpu/generic-instruction.hpp"
#include "test-register-block.hpp"
#include <stdexcept>

class TestInstructionFunctor : public InstructionFunctor {
    using register_t = TestRegisterBlock::RegRef;

  public:
    struct instvalues {
        char opcode;
        char rd;
        char rs1;
        char rs2;
        char imm;
        char minor;

        bool operator==(const instvalues &o) const = default;
    };

    instvalues last;

    void operator()(GenericInstruction &i) {
        auto form = i.get_form().get_form();
        if (form != "TEST")
            throw std::runtime_error(
                std::format("Instruction with inwalid form {} passed", form)
            );
        auto &rd = dynamic_cast<register_t &>(i.get_rd());
        auto &rs1 = dynamic_cast<register_t &>(i.get_rsn(1));
        auto &rs2 = dynamic_cast<register_t &>(i.get_rsn(2));
        last = {
            (char)i.get_opcode(),
            (char)rd.id,
            (char)rs1.id,
            (char)rs2.id,
            (char)i.get_immediate(),
            (char)i.get_minor(),
        };
    }
};
