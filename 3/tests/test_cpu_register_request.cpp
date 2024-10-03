#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cpu/register-set.hpp>
#include <cpu/register-request.hpp>
#include "lib/test-instruction-form.hpp"
#include "lib/test-register-block.hpp"

TEST_CASE("Register request functionality", "[cpu][register_request]") {
    using namespace Catch::Matchers;
    // having test register block
    TestRegisterBlock regs(32);

    // having form of instruction
    SECTION("Construct with rd and 3 rs") {
        InstructionFormMock form(3, {2, 3, 6}, regs);
        {
            RegisterSet set = form.get_registers(0);
            RegisterRequest req(set);
            REQUIRE(regs.references.size() == 4);

            REQUIRE_FALSE(req.is_ready());
            REQUIRE_FALSE(req.is_register_requested(8));
            REQUIRE_FALSE(req.is_register_requested(34));
            REQUIRE(req.is_register_requested(3));
            REQUIRE(req.is_register_requested(2));
            REQUIRE(req.is_register_requested(6));

            auto vect = req.registers();
            REQUIRE(regs.references.size() == 4);
            REQUIRE(vect.size() == 4);
            REQUIRE(vect[0]->get_register_id() == 3);
            REQUIRE(vect[1]->get_register_id() == 2);

            for (auto rg : vect) regs.public_borrow(*rg);

            REQUIRE(req.is_ready());
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct with with 2 rs") {
        InstructionFormMock form(-1, {2, 3}, regs);
        {
            RegisterSet set = form.get_registers(0);
            RegisterRequest req(set);
            REQUIRE(regs.references.size() == 2);

            REQUIRE_FALSE(req.is_ready());
            REQUIRE_FALSE(req.is_register_requested(8));
            REQUIRE_FALSE(req.is_register_requested(34));
            REQUIRE(req.is_register_requested(3));
            REQUIRE(req.is_register_requested(2));

            auto vect = req.registers();
            REQUIRE(regs.references.size() == 2);
            REQUIRE(vect.size() == 2);
            REQUIRE(vect[0]->get_register_id() == 2);
            REQUIRE(vect[1]->get_register_id() == 3);

            for (auto rg : vect) regs.public_borrow(*rg);

            REQUIRE(req.is_ready());
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct rd only") {
        InstructionFormMock form(1, {}, regs);
        {
            RegisterSet set = form.get_registers(0);
            RegisterRequest req(set);
            REQUIRE(regs.references.size() == 1);

            REQUIRE_FALSE(req.is_ready());
            REQUIRE(req.is_register_requested(1));

            auto vect = req.registers();
            REQUIRE(regs.references.size() == 1);
            REQUIRE(vect.size() == 1);
            REQUIRE(vect[0]->get_register_id() == 1);

            for (auto rg : vect) regs.public_borrow(*rg);

            REQUIRE(req.is_ready());
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct empty") {
        InstructionFormMock form(-1, {}, regs);
        {
            RegisterSet set = form.get_registers(0);
            RegisterRequest req(set);
            REQUIRE(regs.references.empty());

            REQUIRE(req.is_ready());

            auto vect = req.registers();
            REQUIRE(regs.references.empty());
            REQUIRE(vect.empty());
        }
        REQUIRE(regs.references.empty());
    }
}
