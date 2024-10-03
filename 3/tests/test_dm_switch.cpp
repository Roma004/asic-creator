#include "data-master/abstracts/abstract-packet.hpp"
#include "data-master/abstracts/endpoints.hpp"
#include "data-master/abstracts/transfer-interface.hpp"
#include "data-master/engine/default-device.hpp"
#include "data-master/engine/device-gate.hpp"
#include "data-master/engine/switch.hpp"
#include "data-master/packet-queue.hpp"
#include "lib/test-packet-data.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdarg>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include <functional>
#include <initializer_list>
#include <memory>
#include <mutex>

void transfer_all(
    int n, std::initializer_list<std::reference_wrapper<TransferInterface>> trns
) {
    for (int i = 0; i < n; ++i) {
        for (auto &trn : trns) trn.get().transfer();
    }
}

using ap_ptr = std::shared_ptr<AbstractPacket>;
using ip_ptr = std::shared_ptr<InterconectPacket>;

bool always(ap_ptr) { return true; }
bool never(ap_ptr) { return false; }
bool is_write_pkt(ap_ptr pkt) { return pkt->get_type() == WRITE; }
bool is_read_pkt(ap_ptr pkt) { return pkt->get_type() == READ; }

ap_ptr
recv_from_slave_ok(SlaveInterface &slv, Sender::cond_t recv_if = always) {
    ap_ptr pkt;
    REQUIRE(slv.get_sender().recv(pkt, recv_if));
    return std::move(pkt);
}

void recv_from_slave_fail(
    SlaveInterface &slv, Sender::cond_t recv_if = always
) {
    ap_ptr pkt;
    REQUIRE_FALSE(slv.get_sender().recv(pkt, recv_if));
}

ap_ptr recv_by_slave_ok(EndpointInterface &slv) {
    ap_ptr pkt;
    REQUIRE(slv.recv_request_pkt(pkt));
    return std::move(pkt);
}

void recv_by_slave_fail(EndpointInterface &slv) {
    ap_ptr pkt;
    REQUIRE_FALSE(slv.recv_request_pkt(pkt));
}

class Resender : public TransferInterface {
  public:
    DeviceGate &dev;
    uint32_t shift;

    Resender(DeviceGate &dev, uint32_t shift) : dev(dev), shift(shift) {}

    void transfer() override {
        auto pkt = recv_by_slave_ok(dev);

        auto &&[new_pkt, data] = make_packet(
            pkt,
            dynamic_cast<const TestPacketData &>(pkt->get_data()).data + shift
        );

        dev.send_response_pkt(new_pkt);
    }
};

void test_pkt_resended(ap_ptr pkt1, ap_ptr pkt2, Resender &r) {
    REQUIRE(
        dynamic_cast<const TestPacketData &>(pkt1->get_data()).data + r.shift
        == dynamic_cast<const TestPacketData &>(pkt2->get_data()).data
    );
}

void test_pkt_is_oops(ap_ptr pkt) {
    REQUIRE(pkt->get_data().type_id() == "OOPS");
}

TEST_CASE("Working with Switch", "[dm][switch]") {
    std::mutex in_m, out_m;
    PacketQueue swc_in_q(in_m), swc_out_q(out_m);
    PacketQueue ds_in_q(in_m), ds_out_q(out_m);

    PacketQueue dev1_in_q(in_m), dev1_out_q(out_m);
    PacketQueue dev2_in_q(in_m), dev2_out_q(out_m);
    PacketQueue dev3_in_q(in_m), dev3_out_q(out_m);

    DefaultDevice def_slv(ds_in_q, ds_out_q);

    Switch swc(1, swc_in_q, swc_out_q, def_slv);

    SECTION("Ranges connection") {
        SECTION("Base address of range is 4-modular") {
            DeviceGate dev1(dev1_in_q, dev1_out_q);
            REQUIRE_THROWS(swc.connect_slave({0x1, 0x100}, dev1));
            REQUIRE_THROWS(swc.connect_slave({0x2, 0x100}, dev1));
            REQUIRE_THROWS(swc.connect_slave({0x3, 0x100}, dev1));
            REQUIRE_NOTHROW(swc.connect_slave({0x0, 0x100}, dev1));
        }

        SECTION("Size of range is 4-modular") {
            DeviceGate dev1(dev1_in_q, dev1_out_q);
            REQUIRE_THROWS(swc.connect_slave({0x0, 0x101}, dev1));
            REQUIRE_THROWS(swc.connect_slave({0x0, 0x102}, dev1));
            REQUIRE_THROWS(swc.connect_slave({0x0, 0x103}, dev1));
            REQUIRE_NOTHROW(swc.connect_slave({0x0, 0x100}, dev1));
        }

        SECTION("Variant insert sequence (all valid)") {
            DeviceGate dev(dev1_in_q, dev1_out_q);

            SECTION("3 ranges from lower to higher addresses") {
                REQUIRE_NOTHROW(swc.connect_slave({0x0, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x100, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x200, 0x100}, dev));
            }

            SECTION("3 ranges from higher to lower addresses") {
                REQUIRE_NOTHROW(swc.connect_slave({0x200, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x100, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x0, 0x100}, dev));
            }
            SECTION("4 ranges no order") {
                REQUIRE_NOTHROW(swc.connect_slave({0x100, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x0, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x300, 0x100}, dev));
                REQUIRE_NOTHROW(swc.connect_slave({0x200, 0x100}, dev));
            }
        }

        SECTION("Fails if ranges are intersecting each other") {
            DeviceGate dev(dev1_in_q, dev1_out_q);
            REQUIRE_NOTHROW(swc.connect_slave({0x4, 0x100}, dev));
            REQUIRE_NOTHROW(swc.connect_slave({0x200, 0x100}, dev));

            SECTION("Connect the same range twice") {
                REQUIRE_THROWS(swc.connect_slave({0x4, 0x100}, dev));
                REQUIRE_THROWS(swc.connect_slave({0x200, 0x100}, dev));
            }

            SECTION("Connect partly intersect, start addr of new is inside of "
                    "existed and offsets are same") {
                REQUIRE_THROWS(swc.connect_slave({0x8, 0x100}, dev));
                REQUIRE_THROWS(swc.connect_slave({0x204, 0x100}, dev));
            }

            SECTION("Connect partly intersect, start addr of new is outside of "
                    "existed but intersects by length") {
                REQUIRE_THROWS(swc.connect_slave({0x0, 0x100}, dev));
                REQUIRE_THROWS(swc.connect_slave({0x204, 0x100}, dev));
            }

            SECTION("New range is fully inside existed one") {
                REQUIRE_THROWS(swc.connect_slave({0x8, 0x10}, dev));
                REQUIRE_THROWS(swc.connect_slave({0x204, 0x10}, dev));
            }

            SECTION("Existed range is fully inside new one") {
                REQUIRE_THROWS(swc.connect_slave({0x0, 0x180}, dev));
                REQUIRE_THROWS(swc.connect_slave({0x1fc, 0x180}, dev));
            }
        }
    }

    SECTION("transition of packets") {

        SECTION("Test for switch with no ranges") {
            auto &&[pkt1, data1] = make_packet(WRITE, 0x0, 123);
            auto &&[pkt2, data2] = make_packet(WRITE, 0x100, 123);

            swc.get_receiver().send(pkt1);
            swc.get_receiver().send(pkt2);

            transfer_all(3, {swc, def_slv});

            auto res_pkt1 = recv_from_slave_ok(swc);
            auto res_pkt2 = recv_from_slave_ok(swc);

            test_pkt_is_oops(res_pkt1);
            test_pkt_is_oops(res_pkt2);
        }

        SECTION("Test for general case switch") {
            DeviceGate dev1(dev1_in_q, dev1_out_q);
            DeviceGate dev2(dev2_in_q, dev2_out_q);
            DeviceGate dev3(dev3_in_q, dev3_out_q);

            Resender r1(dev1, 1), r2(dev2, 2), r3(dev3, 3);

            swc.connect_slaves({
                {{0x100, 0x100}, dev1},
                {{0x200, 0x100}, dev2},
                {{0x400, 0x100}, dev3},
            });

            SECTION("transition to empty space gives oops") {
                auto &&[pkt1, data1] = make_packet(WRITE, 0x0, 123);
                auto &&[pkt2, data2] = make_packet(WRITE, 0x300, 123);
                auto &&[pkt3, data3] = make_packet(WRITE, 0x500, 123);
                swc.get_receiver().send(pkt1);
                swc.get_receiver().send(pkt2);
                swc.get_receiver().send(pkt3);

                transfer_all(3, {swc, def_slv});
                transfer_all(1, {swc});

                auto res1 = recv_from_slave_ok(swc);
                auto res2 = recv_from_slave_ok(swc);
                auto res3 = recv_from_slave_ok(swc);

                test_pkt_is_oops(res1);
                test_pkt_is_oops(res2);
                test_pkt_is_oops(res3);
            }

            SECTION("transition to valid address gives valid result") {
                auto &&[pkt1, data1] = make_packet(WRITE, 0x100, 0x10);
                auto &&[pkt2, data2] = make_packet(WRITE, 0x200, 0x10);
                auto &&[pkt3, data3] = make_packet(WRITE, 0x400, 0x10);

                swc.get_receiver().send(pkt1);
                swc.get_receiver().send(pkt2);
                swc.get_receiver().send(pkt3);

                transfer_all(1, {swc, r1});
                transfer_all(1, {swc, r2});
                transfer_all(1, {swc, r3});
                transfer_all(1, {swc});

                auto res1 = recv_from_slave_ok(swc, [](auto pkt) {
                    return pkt->get_addr() == 0x100;
                });
                auto res2 = recv_from_slave_ok(swc, [](auto pkt) {
                    return pkt->get_addr() == 0x200;
                });
                auto res3 = recv_from_slave_ok(swc, [](auto pkt) {
                    return pkt->get_addr() == 0x400;
                });

                test_pkt_resended(pkt1, res1, r1);
                test_pkt_resended(pkt2, res2, r2);
                test_pkt_resended(pkt3, res3, r3);
            }

            SECTION("transition to address not modular sizeof_data gives oops"
            ) {
                auto &&[pkt1, data1] = make_packet(WRITE, 0x101, 0x10);
                auto &&[pkt2, data2] = make_packet(WRITE, 0x102, 0x10);
                auto &&[pkt3, data3] = make_packet(WRITE, 0x103, 0x10);

                swc.get_receiver().send(pkt1);
                swc.get_receiver().send(pkt2);
                swc.get_receiver().send(pkt3);

                transfer_all(3, {swc, def_slv});
                transfer_all(1, {swc});

                auto res1 = recv_from_slave_ok(swc);
                auto res2 = recv_from_slave_ok(swc);
                auto res3 = recv_from_slave_ok(swc);

                test_pkt_is_oops(res1);
                test_pkt_is_oops(res2);
                test_pkt_is_oops(res3);
            }
        }
    }
}
