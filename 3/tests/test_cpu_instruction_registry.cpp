#include "asic-engine/cpu/base-instruction.hpp"
#include "asic-engine/cpu/generic-instruction.hpp"
#include "asic-engine/cpu/register-request-manager.hpp"
#include "asic-engine/cpu/registries/instruction-registry.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <synk-executor-device/synk-executor-device.hpp>
#include "lib/test-instruction-form.hpp"
#include "lib/test-instruction-functor.hpp"
#include "lib/test-register-block.hpp"

TEST_CASE("Instruction registry functionality", "[cpu][instruction_registry]") {
    std::mutex m;
    InstructionRegistry r;
    RegisterRequestsManager rm(m);

    TestRegisterBlock regs(32);
    InstructionFormTest form(regs, regs);
    TestInstructionFunctor funct1;
    TestInstructionFunctor funct2;
    SynkExecutorDevice exec1(r, rm, m);
    SynkExecutorDevice exec2(r, rm, m);

    r.add_instruction(1, form, funct1, exec1);
    r.add_instruction(2, form, funct2, exec2);

    REQUIRE_THROWS(r.add_instruction(2, form, funct1, exec1));

    REQUIRE(&r.get_executor(BaseInstruction(1)) == &exec1);
    REQUIRE(&r.get_functor(BaseInstruction(1)) == &funct1);
    REQUIRE(&r.get_executor(BaseInstruction(2)) == &exec2);
    REQUIRE(&r.get_functor(BaseInstruction(2)) == &funct2);

    REQUIRE_THROWS(r.get_functor(BaseInstruction(3)));
    REQUIRE_THROWS(r.get_executor(BaseInstruction(3)));

    GenericInstruction gi = r.parse_instruction(BaseInstruction(1));

    REQUIRE(&r.get_functor(gi) == &funct1);
}
