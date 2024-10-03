#include "cpu/base-instruction.hpp"
#include "cpu/generic-instruction.hpp"
#include "cpu/register-request-manager.hpp"
#include "lib/test-instruction-form.hpp"
#include "lib/test-register-block.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cpu/register-request.hpp>
#include <cpu/register-set.hpp>
#include <cstdint>

TEST_CASE("Generic Instruction functionality", "[cpu][generic_instruction]") {
    using namespace Catch::Matchers;
    TestRegisterBlock regs_rd(32);
    TestRegisterBlock regs_rs(32);
    RegisterRequestsManager req_manager;

    SECTION("GenericInstruction with InstructionFormTest") {
        InstructionFormTest form(regs_rd, regs_rs);

        InstructionFormTest::descriptor instr = {
            .opcode = 0x7b,
            .rd = 1,
            .rs1 = 3,
            .rs2 = 4,
            .imm = 0x7f,
            .minor = 0x3,
        };

        BaseInstruction bi = (uint32_t)instr;

        GenericInstruction i(form, bi);

        SECTION("Check for instruction parameters") {
            REQUIRE(i.get_form().get_form() == "TEST");
            REQUIRE(
                i.get_instruction().get_instruction() == bi.get_instruction()
            );
            REQUIRE(i.get_immediate() == 0x7f);
            REQUIRE(i.get_opcode() == 0x7b);
            REQUIRE(i.get_minor() == 0x3);
            REQUIRE(i.get_sub_minor() == 0);
        }

        SECTION("Check for instruction registers") {
            REQUIRE(i.get_rd().get_register_id() == 1);
            REQUIRE(i.get_rsn(1).get_register_id() == 3);
            REQUIRE(i.get_rsn(2).get_register_id() == 4);
            REQUIRE_THROWS_WITH(
                i.get_rsn(3),
                ContainsSubstring("regno=3")
                    && ContainsSubstring("not supported")
                    && ContainsSubstring("TEST-instruction")
            );
            REQUIRE_THROWS_WITH(
                i.get_rsn(0),
                ContainsSubstring("regno=0")
                    && ContainsSubstring("not supported")
                    && ContainsSubstring("TEST-instruction")
            );
            REQUIRE(&(i.get_rd().get_register_block()) == &regs_rd);
            REQUIRE(&i.get_rsn(1).get_register_block() == &regs_rs);
            REQUIRE(&i.get_rsn(2).get_register_block() == &regs_rs);
        }

        SECTION("Check for instruction request") {
            RegisterRequest &req = i.get_request();
            REQUIRE_FALSE(req.is_ready());
            REQUIRE_FALSE(i.is_ready());

            req_manager.add_request(req);
            REQUIRE(req.is_ready());
            REQUIRE(i.is_ready());
        }
    }

    SECTION("GenericInstruction with InstructionFormMock") {
        InstructionFormMock form(-1, {2, 3, 4}, regs_rs);
        GenericInstruction i(form, 0);

        SECTION("Check for instruction parameters") {
            REQUIRE(i.get_form().get_form() == "ASD");
            REQUIRE(i.get_instruction().get_instruction() == 0);
            REQUIRE(i.get_immediate() == 0);
            REQUIRE(i.get_opcode() == 0);
            REQUIRE(i.get_minor() == 0);
            REQUIRE(i.get_sub_minor() == 0);
        }

        SECTION("Check for instruction registers") {
            REQUIRE_THROWS_WITH(
                i.get_rd(),
                ContainsSubstring("Destination register")
                    && ContainsSubstring("not supported")
                    && ContainsSubstring("ASD-instruction")
            );
            REQUIRE(i.get_rsn(1).get_register_id() == 2);
            REQUIRE(i.get_rsn(2).get_register_id() == 3);
            REQUIRE(i.get_rsn(3).get_register_id() == 4);
            REQUIRE_THROWS_WITH(
                i.get_rsn(4),
                ContainsSubstring("regno=4")
                    && ContainsSubstring("not supported")
                    && ContainsSubstring("ASD-instruction")
            );
            REQUIRE_THROWS_WITH(
                i.get_rsn(0),
                ContainsSubstring("regno=0")
                    && ContainsSubstring("not supported")
                    && ContainsSubstring("ASD-instruction")
            );
        }

        SECTION("Check for instruction request") {
            RegisterRequest &req = i.get_request();
            REQUIRE_FALSE(req.is_ready());
            REQUIRE_FALSE(i.is_ready());

            req_manager.add_request(req);
            REQUIRE(req.is_ready());
            REQUIRE(i.is_ready());
        }
    }
}
