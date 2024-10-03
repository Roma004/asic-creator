#include "data-master/abstracts/abstract-packet.hpp"
#include "data-master/engine/default-device.hpp"
#include "data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include <memory>

TEST_CASE("Working with DefaultDevice", "[dm][default_device]") {
    std::mutex m;
    PacketQueue in_q(m);
    PacketQueue out_q(m);

    DefaultDevice dev(in_q, out_q);

    auto data = std::make_shared<TestPacketData>(123);
    auto pkt1 = std::make_shared<InterconectPacket>(WRITE, 1, data);
    auto pkt2 = std::make_shared<InterconectPacket>(READ, 1, data);

    dev.get_receiver().send(pkt1);
    dev.get_receiver().send(pkt2);
    dev.transfer();

    std::shared_ptr<AbstractPacket> tmp_pkt1;
    REQUIRE(dev.get_sender().recv(tmp_pkt1, [](auto) { return true; }));
    REQUIRE(*tmp_pkt1 != *pkt1);
    REQUIRE(tmp_pkt1->get_data().type_id() == "OOPS");
    REQUIRE(tmp_pkt1->get_data().sizeof_data() == 0);
    REQUIRE(tmp_pkt1->get_type() == WRITE);

    dev.transfer();

    std::shared_ptr<AbstractPacket> tmp_pkt2;
    REQUIRE(dev.get_sender().recv(tmp_pkt2, [](auto) { return true; }));
    REQUIRE(*tmp_pkt2 != *pkt2);
    REQUIRE(*tmp_pkt2 != *tmp_pkt1);
    REQUIRE(tmp_pkt2->get_data() == tmp_pkt1->get_data());
    REQUIRE(tmp_pkt2->get_data().type_id() == "OOPS");
    REQUIRE(tmp_pkt2->get_type() == READ);
}
