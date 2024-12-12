#include "asic-engine/data-master/abstracts/packet.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <asic-engine/data-master/common.hpp>
#include <modules-common/interconect-packet.hpp>
#include "lib/test-packet-data.hpp"
#include <memory>

TEST_CASE("Working with InterconectPacket", "[dm][interconnect_packet]") {
    SECTION("Initialization by new data") {
        auto data = std::make_shared<TestPacketData>(123);
        InterconectPacket pkt(WRITE, 1, data);

        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(pkt.get_data() == *data);
        REQUIRE(pkt.get_type() == WRITE);

        REQUIRE(pkt.get_addr() == 1);
        pkt.set_addr(2);
        REQUIRE(pkt.get_addr() == 2);

        REQUIRE(pkt.get_switch_context().get_top_id() == 0);

        const InterconectPacket &pkt_ref = pkt;
        REQUIRE(pkt_ref.get_switch_context().get_top_id() == 0);
    }

    SECTION("Initialization by copy") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);
        InterconectPacket pkt1(WRITE, 1, data1);
        InterconectPacket pkt2(pkt1, data1);
        InterconectPacket pkt3(pkt2, data2);

        REQUIRE(test_packet_data_counter == 2);
        REQUIRE(pkt1.get_data() == *data1);
        REQUIRE(pkt2.get_data() == *data1);
        REQUIRE(pkt3.get_data() == *data2);

        REQUIRE(pkt1.get_type() == WRITE);
        REQUIRE(pkt2.get_type() == WRITE);
        REQUIRE(pkt3.get_type() == WRITE);

        REQUIRE(pkt1 == pkt2);
        REQUIRE(pkt1 != pkt3);
    }

    SECTION("Check for equality operator") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);
        InterconectPacket pkt1(WRITE, 1, data1);
        InterconectPacket pkt2(READ, 1, data1);
        InterconectPacket pkt3(WRITE, 2, data1);
        InterconectPacket pkt4(WRITE, 1, data2);
        InterconectPacket pkt5(WRITE, 1, data1);

        REQUIRE(test_packet_data_counter == 2);

        REQUIRE(pkt1 == pkt5);
        REQUIRE(pkt1 != pkt2);
        REQUIRE(pkt1 != pkt3);
        REQUIRE(pkt1 != pkt4);
        REQUIRE(pkt5 != pkt2);
        REQUIRE(pkt5 != pkt3);
        REQUIRE(pkt5 != pkt4);
    }
}
