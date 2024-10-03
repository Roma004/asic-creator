#include "data-master/abstracts/abstract-packet.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include "data-master/engine/cpu-gate.hpp"
#include "data-master/engine/device-gate.hpp"
#include "data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <memory>

TEST_CASE("Working with CPUGate", "[dm][cpu_gate]") {
    std::mutex m;
    PacketQueue in_q(m);
    PacketQueue out_q(m);

    DeviceGate dev(in_q, out_q);
    CPUGate host(dev.get_sender(), dev.get_receiver());

    SECTION("Send request to slave") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);

        host.send_request_pkt(pkt1);

        std::shared_ptr<AbstractPacket> tmp_pkt;
        REQUIRE(dev.recv_request_pkt(tmp_pkt));
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(*tmp_pkt == *pkt1);
    }

    SECTION("Get response from slave") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);

        dev.send_response_pkt(pkt1);

        std::shared_ptr<AbstractPacket> tmp_pkt;

        REQUIRE(host.recv_response_pkt(tmp_pkt));
        REQUIRE(test_packet_data_counter == 1);
        REQUIRE(*tmp_pkt == *pkt1);
    }
}
