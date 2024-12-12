#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <cstdint>

TEST_CASE("Working with BaseInstruction", "[cpu, base_instruction]") {
    uint32_t word1 = 0x7f;
    uint32_t word2 = 0x2;
    uint32_t word3 = 0xf2;
    BaseInstruction i1(word1), i2(word2), i3(word3);

    REQUIRE(i1.get_opcode() == word1);
    REQUIRE(i2.get_opcode() == word2);
    REQUIRE(i3.get_opcode() == 0x72);

    REQUIRE(i1.get_instruction() == word1);
    REQUIRE(i2.get_instruction() == word2);
    REQUIRE(i3.get_instruction() == word3);
}
