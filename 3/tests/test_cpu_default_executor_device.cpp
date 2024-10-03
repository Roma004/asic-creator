#include "cpu/engine/executor-device.hpp"
#include "cpu/registries/instruction-registry.hpp"
#include "cpu/register-request-manager.hpp"
#include "lib/test-instruction-form.hpp"
#include "lib/test-instruction-functor.hpp"
#include "lib/test-register-block.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cpu/register-request.hpp>
#include <cpu/register-set.hpp>

TEST_CASE("Executor Device functionality", "[cpu][executor_device]") {
    using i_vals = TestInstructionFunctor::instvalues;
    RegisterRequestsManager req_manger;

    TestRegisterBlock reg_blk(10);
    InstructionFormTest test_form(reg_blk, reg_blk);

    TestInstructionFunctor funct;

    SECTION("Handle one instruction") {
        InstructionRegistry registry;
        DefaultExecutorDevice dev(registry, req_manger);
        registry.add_instruction(0x7a, test_form, funct, dev);

        dev.add_instruction(test_form.format(0x7a, 1, 1, 2, 3, 7));
        REQUIRE(reg_blk.references.size() == 3);
        dev.handle();

        REQUIRE(funct.last == (i_vals){0x7a, 1, 1, 2, 3, 7});
        REQUIRE(reg_blk.references.size() == 0);
    };

    SECTION("Handle two instructions") {
        InstructionRegistry registry;
        DefaultExecutorDevice dev(registry, req_manger);
        registry.add_instruction(0x7a, test_form, funct, dev);

        dev.add_instruction(test_form.format(0x7a, 1, 1, 2, 3, 7));
        dev.add_instruction(test_form.format(0x7a, 1, 1, 2, 3, 2));

        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 1, 1, 2, 3, 7});

        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 1, 1, 2, 3, 2});
    };

    SECTION("Handle instructions with locks") {
        InstructionRegistry registry;
        DefaultExecutorDevice dev(registry, req_manger);
        registry.add_instruction(0x7a, test_form, funct, dev);

        auto r4 = reg_blk.make_reference(4);
        RegisterSet r4_set(r4);
        RegisterRequest r4_req(r4_set);
        req_manger.add_request(r4_req);

        dev.add_instruction(test_form.format(0x7a, 1, 2, 3, 1, 7));
        dev.add_instruction(test_form.format(0x7a, 5, 6, 4, 1, 6));
        dev.add_instruction(test_form.format(0x7a, 1, 2, 3, 1, 5));

        // first instructuion are handled first
        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 1, 2, 3, 1, 7});

        // skip second instruction, as resources locked, and process the last
        // one
        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 1, 2, 3, 1, 5});

        // resourecs are stil locked
        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 1, 2, 3, 1, 5});

        // handle the second, as resources aquired
        req_manger.free_registers(r4_req);
        dev.handle();
        REQUIRE(funct.last == (i_vals){0x7a, 5, 6, 4, 1, 6});
    };
}
