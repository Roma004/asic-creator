#include "cpu/register-request-manager.hpp"
#include "lib/test-instruction-form.hpp"
#include "lib/test-register-block.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cpu/register-request.hpp>
#include <cpu/register-set.hpp>

TEST_CASE(
    "Register request manager functionality", "[cpu][register_request_manager]"
) {
    using namespace Catch::Matchers;
    // having test register block
    TestRegisterBlock regs_x32(32);
    TestRegisterBlock regs_x64(64);

    InstructionFormMock formA(1, {2, 1, 6}, regs_x32);

    RegisterRequestsManager req_manager;

    SECTION("Single request process") {
        RegisterSet set_a = formA.get_registers(0);
        RegisterRequest a1(set_a);

        req_manager.add_request(a1);
        REQUIRE(a1.is_ready());
        for (auto reg : set_a.registers()) REQUIRE(reg->is_borrowed());

        REQUIRE(regs_x32.regs[1].locked);
        REQUIRE(regs_x32.regs[2].locked);
        REQUIRE(regs_x32.regs[6].locked);

        req_manager.free_registers(a1);
        for (auto reg : set_a.registers()) REQUIRE_FALSE(reg->is_borrowed());

        for (auto reg : regs_x32.regs) REQUIRE_FALSE(reg.locked);
    }

    SECTION("Fully inersect requests process") {
        RegisterSet set_a1 = formA.get_registers(0);
        RegisterSet set_a2 = formA.get_registers(0);
        RegisterRequest a1(set_a1);
        RegisterRequest a2(set_a2);

        req_manager.add_request(a1);
        req_manager.add_request(a2);
        REQUIRE(a1.is_ready());
        REQUIRE_FALSE(a2.is_ready());

        REQUIRE(regs_x32.regs[1].locked);
        REQUIRE(regs_x32.regs[2].locked);
        REQUIRE(regs_x32.regs[6].locked);

        for (auto reg : set_a1.registers()) REQUIRE(reg->is_borrowed());
        for (auto reg : set_a2.registers()) REQUIRE_FALSE(reg->is_borrowed());

        req_manager.free_registers(a1);
        REQUIRE_FALSE(a1.is_ready());
        REQUIRE(a2.is_ready());

        REQUIRE(regs_x32.regs[1].locked);
        REQUIRE(regs_x32.regs[2].locked);
        REQUIRE(regs_x32.regs[6].locked);

        for (auto reg : set_a1.registers()) REQUIRE_FALSE(reg->is_borrowed());
        for (auto reg : set_a2.registers()) REQUIRE(reg->is_borrowed());

        req_manager.free_registers(a2);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE_FALSE(a2.is_ready());

        for (auto reg : set_a1.registers()) REQUIRE_FALSE(reg->is_borrowed());
        for (auto reg : set_a2.registers()) REQUIRE_FALSE(reg->is_borrowed());
        for (auto reg : regs_x32.regs) REQUIRE_FALSE(reg.locked);
    }

    // InstructionFormMock formA(1, {2, 1, 6}, regs_x32);
    InstructionFormMock formB(2, {6, 7}, regs_x32);
    InstructionFormMock formC(3, {9, 3, 4}, regs_x32);
    InstructionFormMock formD(-1, {1, 8, 7, 9}, regs_x32);

    SECTION("Partly inersect requests process") {
        /*
         * |##|1|2|3|4|5|6|7|8|9|
         * |--|-|-|-|-|-|-|-|-|-|
         * |a1|x|x| | | |x| | | |
         * |--|-|-|-|-|-|-|-|-|-|
         * |b1| |x| | | |x|x| | |
         * |--|-|-|-|-|-|-|-|-|-|
         * |c1| | |x|x| | | | |x|
         * |--|-|-|-|-|-|-|-|-|-|
         * |d1|x| | | | | |x|x|x|
         * |--|-|-|-|-|-|-|-|-|-|
         * |d2|x| | | | | |x|x|x|
         * |--|-|-|-|-|-|-|-|-|-|
         * a -- b --.
         *           \
         * c ---------d
         * */
        RegisterSet set_a = formA.get_registers(0);
        RegisterSet set_b = formB.get_registers(0);
        RegisterSet set_c = formC.get_registers(0);
        RegisterSet set_d1 = formD.get_registers(0);
        RegisterSet set_d2 = formD.get_registers(0);

        RegisterRequest a1(set_a);
        RegisterRequest b1(set_b);
        RegisterRequest c1(set_c);
        RegisterRequest d1(set_d1);
        RegisterRequest d2(set_d2);

        req_manager.add_request(a1);
        req_manager.add_request(b1);
        req_manager.add_request(c1);
        req_manager.add_request(d1);
        req_manager.add_request(d2);

        REQUIRE(a1.is_ready());
        REQUIRE_FALSE(b1.is_ready());
        REQUIRE(c1.is_ready());
        REQUIRE_FALSE(d1.is_ready());
        REQUIRE_FALSE(d2.is_ready());

        for (auto i : {1,2,3,4,6,7,8,9}) REQUIRE(regs_x32.regs[i].locked);
        for (auto i : {5}) REQUIRE_FALSE(regs_x32.regs[i].locked);

        req_manager.free_registers(a1);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE(b1.is_ready());
        REQUIRE(c1.is_ready());
        REQUIRE_FALSE(d1.is_ready());
        REQUIRE_FALSE(d2.is_ready());

        for (auto i : {1,2,3,4,6,7,8,9}) REQUIRE(regs_x32.regs[i].locked);
        for (auto i : {5}) REQUIRE_FALSE(regs_x32.regs[i].locked);

        req_manager.free_registers(c1);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE(b1.is_ready());
        REQUIRE_FALSE(c1.is_ready());
        REQUIRE_FALSE(d1.is_ready());
        REQUIRE_FALSE(d2.is_ready());

        for (auto i : {1,2,6,7,8,9}) REQUIRE(regs_x32.regs[i].locked);
        for (auto i : {3,4,5}) REQUIRE_FALSE(regs_x32.regs[i].locked);

        req_manager.free_registers(b1);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE_FALSE(b1.is_ready());
        REQUIRE_FALSE(c1.is_ready());
        REQUIRE(d1.is_ready());
        REQUIRE_FALSE(d2.is_ready());

        for (auto i : {1,7,8,9}) REQUIRE(regs_x32.regs[i].locked);
        for (auto i : {2,3,4,5,6}) REQUIRE_FALSE(regs_x32.regs[i].locked);

        req_manager.free_registers(d1);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE_FALSE(b1.is_ready());
        REQUIRE_FALSE(c1.is_ready());
        REQUIRE_FALSE(d1.is_ready());
        REQUIRE(d2.is_ready());

        for (auto i : {1,7,8,9}) REQUIRE(regs_x32.regs[i].locked);
        for (auto i : {2,3,4,5,6}) REQUIRE_FALSE(regs_x32.regs[i].locked);

        req_manager.free_registers(d2);

        REQUIRE_FALSE(a1.is_ready());
        REQUIRE_FALSE(b1.is_ready());
        REQUIRE_FALSE(c1.is_ready());
        REQUIRE_FALSE(d1.is_ready());
        REQUIRE_FALSE(d2.is_ready());

        for (auto reg : regs_x32.regs) REQUIRE_FALSE(reg.locked);
    }

    SECTION("Inersect indexes but not blocks request process") {}
    SECTION("Complex case") {}
}
