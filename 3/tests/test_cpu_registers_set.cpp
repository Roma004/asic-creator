#include "lib/test-instruction-form.hpp"
#include <asic-engine/cpu/register-set.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE("Register set functionality", "[cpu][register_set]") {
    using namespace Catch::Matchers;
    // having test register block
    TestRegisterBlock regs(32);

    // having form of instruction
    SECTION("Construct with rd with 3 rs") {
        InstructionFormMock form(3, {2, 3, 6}, regs);
        {
            RegisterSet set = form.get_registers(0);
            REQUIRE(regs.references.size() == 4);

            REQUIRE(set.has_rd());
            REQUIRE(set.has_rsn(1));
            REQUIRE(set.has_rsn(2));
            REQUIRE(set.has_rsn(3));
            REQUIRE(!set.has_rsn(4));

            REQUIRE(set.get_rd()->get_register_id() == 3);
            REQUIRE(set.get_rsn(1)->get_register_id() == 2);
            REQUIRE(set.get_rsn(2)->get_register_id() == 3);
            REQUIRE(set.get_rsn(3)->get_register_id() == 6);
            REQUIRE_THROWS_WITH(set.get_rsn(4), ContainsSubstring("rs4"));

            auto vect = set.registers();
            REQUIRE(regs.references.size() == 4);
            REQUIRE(vect.size() == 4);
            REQUIRE(vect[0]->get_register_id() == 3);
            REQUIRE(vect[1]->get_register_id() == 2);
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct with with 2 rs") {
        InstructionFormMock form(-1, {2, 3}, regs);
        {
            RegisterSet set = form.get_registers(0);
            REQUIRE(regs.references.size() == 2);

            REQUIRE(!set.has_rd());
            REQUIRE(set.has_rsn(1));
            REQUIRE(set.has_rsn(2));
            REQUIRE(!set.has_rsn(3));

            REQUIRE_THROWS_WITH(set.get_rd(), ContainsSubstring("rd"));
            REQUIRE(set.get_rsn(1)->get_register_id() == 2);
            REQUIRE(set.get_rsn(2)->get_register_id() == 3);
            REQUIRE_THROWS_WITH(set.get_rsn(3), ContainsSubstring("rs3"));

            auto vect = set.registers();
            REQUIRE(regs.references.size() == 2);
            REQUIRE(vect.size() == 2);
            REQUIRE(vect[0]->get_register_id() == 2);
            REQUIRE(vect[1]->get_register_id() == 3);
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct invalid (operand nullptr)") {
        InstructionFormMock form(-1, {1, 3, -1, 5}, regs);
        REQUIRE_THROWS_WITH(
            form.get_registers(0), ContainsSubstring("nullptr is not allowed")
        );
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct invalid (bad iterators)") {
        InstructionFormMock form(-1, {1, 3, 4, 5}, regs);
        REQUIRE_THROWS_WITH(
            form.get_registers(1), ContainsSubstring("negative distance")
        );
        REQUIRE_NOTHROW(form.get_registers(0));
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct rd only") {
        InstructionFormMock form(1, {}, regs);
        {
            RegisterSet set = form.get_registers(0);
            REQUIRE(regs.references.size() == 1);

            REQUIRE(set.get_rs_number() == 0);
            REQUIRE(set.has_rd());
            REQUIRE(!set.has_rsn(0));

            REQUIRE(set.get_rd()->get_register_id() == 1);
            REQUIRE_THROWS_WITH(set.get_rsn(1), ContainsSubstring("rs1"));

            auto vect = set.registers();
            REQUIRE(regs.references.size() == 1);
            REQUIRE(vect.size() == 1);
            REQUIRE(vect[0]->get_register_id() == 1);
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct empty") {
        InstructionFormMock form(-1, {}, regs);
        {
            RegisterSet set = form.get_registers(0);
            REQUIRE(regs.references.empty());

            REQUIRE(set.get_rs_number() == 0);
            REQUIRE(!set.has_rd());
            REQUIRE(!set.has_rsn(0));
            REQUIRE(!set.has_rsn(1));
            REQUIRE(!set.has_rsn(2));

            REQUIRE_THROWS_WITH(set.get_rd(), ContainsSubstring("rd"));
            REQUIRE_THROWS_WITH(set.get_rsn(1), ContainsSubstring("rs1"));
            REQUIRE_THROWS_WITH(set.get_rsn(2), ContainsSubstring("rs2"));
            REQUIRE_THROWS_WITH(
                set.get_rsn(0), ContainsSubstring("numbers from 1")
            );

            auto vect = set.registers();
            REQUIRE(regs.references.empty());
            REQUIRE(vect.size() == 0);
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct manualy with rd") {
        {
            auto rd = regs.make_reference(1);
            auto rs1 = regs.make_reference(3);
            auto rs2 = regs.make_reference(4);
            RegisterSet set(rd, {rs1, rs2});

            REQUIRE(regs.references.size() == 3);

            REQUIRE(set.get_rs_number() == 2);
            REQUIRE(set.has_rd());
            REQUIRE(!set.has_rsn(0));
            REQUIRE(set.has_rsn(1));
            REQUIRE(set.has_rsn(2));
            REQUIRE(!set.has_rsn(3));

            REQUIRE_THROWS_WITH(set.get_rsn(3), ContainsSubstring("rs3"));
            REQUIRE_THROWS_WITH(
                set.get_rsn(0), ContainsSubstring("numbers from 1")
            );

            auto vect = set.registers();
            REQUIRE(regs.references.size() == 3);
            REQUIRE(vect.size() == 3);
            REQUIRE(vect[0] == rd);
            REQUIRE(vect[1] == rs1);
            REQUIRE(vect[2] == rs2);
        }
        REQUIRE(regs.references.empty());
    }

    SECTION("Construct manualy without rd") {
        {
            auto rs1 = regs.make_reference(3);
            auto rs2 = regs.make_reference(4);
            RegisterSet set({rs1, rs2});

            REQUIRE(regs.references.size() == 2);

            REQUIRE(set.get_rs_number() == 2);
            REQUIRE(!set.has_rd());
            REQUIRE(!set.has_rsn(0));
            REQUIRE(set.has_rsn(1));
            REQUIRE(set.has_rsn(2));
            REQUIRE(!set.has_rsn(3));

            REQUIRE_THROWS_WITH(set.get_rd(), ContainsSubstring("rd"));
            REQUIRE_THROWS_WITH(set.get_rsn(3), ContainsSubstring("rs3"));
            REQUIRE_THROWS_WITH(
                set.get_rsn(0), ContainsSubstring("numbers from 1")
            );

            auto vect = set.registers();
            REQUIRE(regs.references.size() == 2);
            REQUIRE(vect.size() == 2);
            REQUIRE(vect[0] == rs1);
            REQUIRE(vect[1] == rs2);
        }
        REQUIRE(regs.references.empty());
    }
}
