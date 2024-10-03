#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include "data-master/abstracts/abstract-packet.hpp"
#include "data-master/engine/packet-sender.hpp"
#include "data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <memory>

TEST_CASE("Working with PacketSender", "[dm][packet_sender]") {
    std::mutex m;
    PacketQueue q(m);

    PacketSender r(q);

    SECTION("Extraction of packets") {
        auto data = std::make_shared<TestPacketData>(123);
        auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);
        auto pkt2 = std::make_shared<InterconectPacket>(READ, 1, data);

        q.push(pkt1);
        q.push(pkt2);

        std::shared_ptr<AbstractPacket> tmp_pkt;

        SECTION("Receive everything") {
            REQUIRE(r.recv(tmp_pkt));
            REQUIRE(test_packet_data_counter == 1);
            REQUIRE(tmp_pkt->get_data() == *data);
            REQUIRE(tmp_pkt->get_type() == WRITE);

            REQUIRE(r.recv(tmp_pkt));
            REQUIRE(test_packet_data_counter == 1);
            REQUIRE(tmp_pkt->get_data() == *data);
            REQUIRE(tmp_pkt->get_type() == READ);
        }

        SECTION("Receive only WRITE") {
            auto is_write = [](auto pkt) { return pkt->get_type() == WRITE; };

            REQUIRE(r.recv(tmp_pkt, is_write));
            REQUIRE(test_packet_data_counter == 1);
            REQUIRE(tmp_pkt->get_data() == *data);
            REQUIRE(tmp_pkt->get_type() == WRITE);

            REQUIRE_FALSE(r.recv(tmp_pkt, is_write));
            REQUIRE_FALSE(q.empty());
        }

        SECTION("Receive only READ") {
            auto is_read = [](auto pkt) { return pkt->get_type() == READ; };

            REQUIRE_FALSE(r.recv(tmp_pkt, is_read));
            q.pop(tmp_pkt);
            REQUIRE(r.recv(tmp_pkt, is_read));
            REQUIRE(test_packet_data_counter == 1);
            REQUIRE(tmp_pkt->get_data() == *data);
            REQUIRE(tmp_pkt->get_type() == READ);
        }
    }
}
