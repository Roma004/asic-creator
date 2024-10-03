#include "data-master/abstracts/abstract-packet.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include "data-master/engine/device-gate.hpp"
#include "data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <memory>

TEST_CASE("Working with DeviceGate", "[dm][device_gate]") {
    std::mutex m;
    PacketQueue in_q(m);
    PacketQueue out_q(m);

    DeviceGate gate(in_q, out_q);

    SECTION("Receive request from master") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);

        gate.get_receiver().send(pkt1);

        std::shared_ptr<AbstractPacket> tmp_pkt;
        REQUIRE(gate.recv_request_pkt(tmp_pkt));
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(*tmp_pkt == *pkt1);
    }

    SECTION("Send response to master") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);

        gate.send_response_pkt(pkt1);

        std::shared_ptr<AbstractPacket> tmp_pkt;
        auto is_write = [](auto pkt) { return pkt->get_type() == WRITE; };
        auto is_read = [](auto pkt) { return pkt->get_type() == READ; };

        REQUIRE_FALSE(gate.get_sender().recv(tmp_pkt, is_read));
        REQUIRE(gate.get_sender().recv(tmp_pkt, is_write));
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(*tmp_pkt == *pkt1);
    }
}
