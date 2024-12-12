#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <asic-engine/data-master/common.hpp>
#include <modules-common/interconect-packet.hpp>
#include "modules-common/packet-receiver.hpp"
#include "asic-engine/data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <memory>

TEST_CASE("Working with PacketReceiver", "[dm][packet_receiver]") {
    std::mutex m;
    PacketQueue q(m);

    PacketReceiver r(q);

    SECTION("Insertion of packets") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);
        auto pkt2 = std::make_shared<InterconectPacket>(READ, 1, data);

        r.send(pkt1);
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(q.front()->get_data() == *data);
        REQUIRE(q.front()->get_type() == WRITE);

        r.send(pkt2);
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(q.back()->get_data() == *data);
        REQUIRE(q.back()->get_type() == READ);
    }
}
