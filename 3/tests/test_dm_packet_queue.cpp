#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <asic-engine/data-master/common.hpp>
#include <modules-common/interconect-packet.hpp>
#include <memory>

TEST_CASE("Working with PacketQueue", "[dm][packet_queue]") {
    std::mutex m;
    PacketQueue q(m);

    SECTION("ThreadSafeQueue::push() lvalue") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);

        auto pkt_ptr1 = std::make_shared<InterconectPacket>(WRITE, 1, data1);
        auto pkt_ptr2 = std::make_shared<InterconectPacket>(READ, 1, data2);

        REQUIRE(test_packet_data_counter == 2);

        REQUIRE(q.empty());

        const std::shared_ptr<PacketInterface> &pkt_ref1 = pkt_ptr1;
        const std::shared_ptr<PacketInterface> &pkt_ref2 = pkt_ptr2;

        q.push(pkt_ref1);

        REQUIRE(test_packet_data_counter == 2);

        SECTION("check after first push") {
            REQUIRE_FALSE(q.empty());

            auto front = q.front();
            auto back = q.back();

            REQUIRE(front->get_data() == *data1);
            REQUIRE(back->get_data() == *data1);
        }

        q.push(pkt_ref2);

        REQUIRE(test_packet_data_counter == 2);

        SECTION("check after second push") {
            REQUIRE_FALSE(q.empty());

            auto front = q.front();
            auto back = q.back();

            REQUIRE(front->get_data() == *data1);
            REQUIRE(back->get_data() == *data2);
        }
    }

    SECTION("ThreadSafeQueue::push() rvalue") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);

        auto pkt_ptr1 = std::make_shared<InterconectPacket>(WRITE, 1, data1);
        auto pkt_ptr2 = std::make_shared<InterconectPacket>(READ, 1, data2);

        REQUIRE(test_packet_data_counter == 2);

        REQUIRE(q.empty());

        q.push(std::move(pkt_ptr1));

        REQUIRE(test_packet_data_counter == 2);

        SECTION("check after first push") {
            REQUIRE_FALSE(q.empty());

            auto front = q.front();
            auto back = q.back();

            REQUIRE(front->get_data() == *data1);
            REQUIRE(back->get_data() == *data1);
        }

        q.push(std::move(pkt_ptr2));

        REQUIRE(test_packet_data_counter == 2);

        SECTION("check after second push") {
            REQUIRE_FALSE(q.empty());

            auto front = q.front();
            auto back = q.back();

            REQUIRE(front->get_data() == *data1);
            REQUIRE(back->get_data() == *data2);
        }
    }

    SECTION("ThreadSafeQueue::pop()") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);

        auto pkt_ptr1 = std::make_shared<InterconectPacket>(WRITE, 1, data1);
        auto pkt_ptr2 = std::make_shared<InterconectPacket>(READ, 1, data2);

        REQUIRE(test_packet_data_counter == 2);

        q.push(std::move(pkt_ptr1));
        q.push(std::move(pkt_ptr2));

        REQUIRE(q.front()->get_data() == *data1);

        std::shared_ptr<PacketInterface> tmp_pkt;
        q.pop(tmp_pkt);

        REQUIRE(tmp_pkt->get_data() == *data1);
        REQUIRE(q.front()->get_data() == *data2);

        q.pop(tmp_pkt);

        REQUIRE(tmp_pkt->get_data() == *data2);
        REQUIRE(q.empty());
    }

    SECTION("ThreadSafeQueue::pop_if()") {
        auto data1 = std::make_shared<TestPacketData>(123);
        auto data2 = std::make_shared<TestPacketData>(124);

        auto pkt_ptr1 = std::make_shared<InterconectPacket>(WRITE, 1, data1);
        auto pkt_ptr2 = std::make_shared<InterconectPacket>(READ, 1, data2);

        REQUIRE(test_packet_data_counter == 2);

        auto is_write_pkt = [](auto pkt) { return pkt->get_type() == WRITE; };
        auto is_read_pkt = [](auto pkt) { return pkt->get_type() == READ; };
        std::shared_ptr<PacketInterface> tmp_pkt;

        SECTION("pop_if WRITE for front = WRITE (succussfull pop)") {
            q.push(std::move(pkt_ptr1));
            q.push(std::move(pkt_ptr2));

            REQUIRE(q.size() == 2);

            REQUIRE(q.pop_if(tmp_pkt, is_write_pkt));

            REQUIRE(tmp_pkt->get_data() == *data1);
            REQUIRE(q.front()->get_data() == *data2);
        }

        SECTION("pop_if WRITE for front = READ (unsuccussfull pop)") {
            q.push(std::move(pkt_ptr2));
            q.push(std::move(pkt_ptr1));

            REQUIRE(q.size() == 2);

            REQUIRE_FALSE(q.pop_if(tmp_pkt, is_write_pkt));

            REQUIRE(q.front()->get_data() == *data2);

            REQUIRE(q.size() == 2);
        }

        SECTION("pop_if READ for front = READ (succussfull pop)") {
            q.push(std::move(pkt_ptr1));
            q.push(std::move(pkt_ptr2));

            REQUIRE(q.size() == 2);

            REQUIRE(q.pop_if(tmp_pkt, is_write_pkt));

            REQUIRE(q.size() == 1);

            REQUIRE(tmp_pkt->get_data() == *data1);
            REQUIRE(q.front()->get_data() == *data2);
        }

        SECTION("pop_if makes queue empty") {
            q.push(std::move(pkt_ptr1));

            REQUIRE(q.size() == 1);

            REQUIRE(q.pop_if(tmp_pkt, is_write_pkt));

            REQUIRE(q.size() == 0);
            REQUIRE(q.empty());

            REQUIRE(tmp_pkt->get_data() == *data1);
        }

        SECTION("pop_if from empty queue is false") {
            REQUIRE(q.size() == 0);
            REQUIRE(q.empty());

            REQUIRE_FALSE(q.pop_if(tmp_pkt, is_write_pkt));
        }
    }
}

