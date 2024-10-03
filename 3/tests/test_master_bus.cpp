#include "common.hpp"
#include <endpoints/cpu-gate.hpp>
#include <endpoints/default-device.hpp>
#include <endpoints/device-gate.hpp>
#include <endpoints/switch.hpp>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <mutex>

#define print_pkt(pkt)                           \
    {                                            \
        fprintf(                                 \
            stderr,                              \
            #pkt ": Pkt(%s, 0x%x, 0x%x)\n",      \
            (pkt.ct.type == READ ? "RD" : "WR"), \
            pkt.ct.addr,                         \
            pkt.ct.data                          \
        );                                       \
    }

#define recv_packet(pkt_name_, interface_)   \
    InterconectPacket pkt_name_;             \
    REQUIRE(interface_.recv_pkt(pkt_name_)); \
    print_pkt(pkt_name_);

#define new_queue(name__)  \
    std::mutex name__##_m; \
    InterconectQueue name__##_mosi_q(name__##_m), name__##_miso_q(name__##_m);

#define new_switch(swc_id, name__, def_slv) \
    new_queue(name__);                      \
    Switch name__(swc_id, name__##_mosi_q, name__##_miso_q, def_slv);

#define new_slv(slv_type__, name__) \
    new_queue(name__);              \
    slv_type__ name__(name__##_mosi_q, name__##_miso_q);

#define new_cpu(name__, swc__) CPUGate name__(swc__.mosi, swc__.miso);

#define transfer_pkt              \
    for (int i = 0; i < 4; ++i) { \
        switch1.transfer();       \
        switch2.transfer();       \
        switch3.transfer();       \
        switch4.transfer();       \
        ds.transfer();            \
    }

#define peer_to_peer_pkt_test(from_, to_, g_addr_, addr_, type_, pkt_) \
    InterconectPacket pkt_##_1 = {{type_, g_addr_, 0x0}, SwcFlow()};   \
    from_.send_pkt(pkt_##_1);                                          \
    transfer_pkt;                                                      \
    recv_packet(pkt_##_2, to_);                                        \
    REQUIRE(pkt_##_2.ct.type == pkt_##_1.ct.type);                     \
    REQUIRE(pkt_##_2.ct.addr == addr_);                                \
    REQUIRE(pkt_##_2.ct.data == pkt_##_1.ct.data);                     \
    to_.send_pkt(pkt_##_2);                                            \
    transfer_pkt;                                                      \
    recv_packet(pkt_##_3, from_);                                      \
    REQUIRE(pkt_##_1 == pkt_##_3);

#define bad_address_test(from_, g_addr_, type_, pkt_)                    \
    InterconectPacket pkt_##_bad_1 = {{type_, g_addr_, 0x0}, SwcFlow()}; \
    from_.send_pkt(pkt_##_bad_1);                                        \
    transfer_pkt;                                                        \
    recv_packet(pkt_##_bad_2, from_);                                    \
    REQUIRE(pkt_##_bad_2.ct.type == type_);                              \
    REQUIRE(pkt_##_bad_2.ct.addr == g_addr_);                            \
    REQUIRE(pkt_##_bad_2.ct.data == OOPS);

TEST_CASE("Test for master bus connectivity", "[master_bus_connectivity]") {
    new_slv(DeviceGate, slv1);
    new_slv(DeviceGate, slv2);
    new_slv(DeviceGate, slv3);
    new_slv(DeviceGate, slv4);
    new_slv(DeviceGate, slv5);
    new_slv(DeviceGate, slv6);
    new_slv(DeviceGate, slv7);
    new_slv(DeviceGate, slv8);
    new_slv(DefaultDevice, ds);
    int id = 1;

    new_switch(id++, switch1, ds);
    new_switch(id++, switch2, ds);
    new_switch(id++, switch3, ds);
    new_switch(id++, switch4, ds);

    new_cpu(cpu1, switch1);
    new_cpu(cpu2, switch1);
    new_cpu(cpu3, switch1);

    new_cpu(cpu4, switch4);
    new_cpu(cpu5, switch4);
    new_cpu(cpu6, switch4);
    new_cpu(cpu7, switch4);

    switch1.connect_slaves({
        {{0x100, 0xf00}, slv1},
        {{0x1000, 0x1000}, slv2},
        {{0x2000, 0x6000}, switch2},
    });

    switch2.connect_slaves({
        {{0x100, 0x100}, slv3},
        {{0x3000, 0x3000}, switch3},
    });

    switch3.connect_slaves({
        {{0x100, 0x200}, slv4},
        {{0x300, 0xd00}, slv5},
        {{0x1000, 0x2000}, slv6},
    });

    switch4.connect_slaves({
        {{0x0, 0x6000}, switch2},
        {{0x6000, 0x1000}, slv7},
        {{0x7000, 0x1000}, slv8},
    });

    peer_to_peer_pkt_test(cpu1, slv1, 0x0100, 0x0000, READ, pkt11);
    peer_to_peer_pkt_test(cpu2, slv1, 0x0150, 0x0050, READ, pkt21);
    peer_to_peer_pkt_test(cpu3, slv1, 0x0ffc, 0x0efc, READ, pkt31);

    peer_to_peer_pkt_test(cpu1, slv2, 0x1000, 0x0000, READ, pkt12);
    peer_to_peer_pkt_test(cpu2, slv2, 0x1524, 0x0524, READ, pkt22);
    peer_to_peer_pkt_test(cpu3, slv2, 0x1ffc, 0x0ffc, READ, pkt32);

    peer_to_peer_pkt_test(cpu1, slv3, 0x2100, 0x0000, READ, pkt13);
    peer_to_peer_pkt_test(cpu2, slv3, 0x2150, 0x0050, READ, pkt23);
    peer_to_peer_pkt_test(cpu3, slv3, 0x21fc, 0x00fc, READ, pkt33);

    peer_to_peer_pkt_test(cpu1, slv4, 0x5100, 0x0000, READ, pkt14);
    peer_to_peer_pkt_test(cpu2, slv4, 0x5250, 0x0150, READ, pkt24);
    peer_to_peer_pkt_test(cpu3, slv4, 0x52fc, 0x01fc, READ, pkt34);

    peer_to_peer_pkt_test(cpu1, slv5, 0x5300, 0x0000, READ, pkt15);
    peer_to_peer_pkt_test(cpu2, slv5, 0x5e54, 0x0b54, READ, pkt25);
    peer_to_peer_pkt_test(cpu3, slv5, 0x5ffc, 0x0cfc, READ, pkt35);

    peer_to_peer_pkt_test(cpu1, slv6, 0x6000, 0x0000, READ, pkt16);
    peer_to_peer_pkt_test(cpu2, slv6, 0x7e54, 0x1e54, READ, pkt26);
    peer_to_peer_pkt_test(cpu3, slv6, 0x7ffc, 0x1ffc, READ, pkt36);

    peer_to_peer_pkt_test(cpu4, slv3, 0x0100, 0x0000, READ, pkt43);
    peer_to_peer_pkt_test(cpu5, slv3, 0x0154, 0x0054, READ, pkt53);
    peer_to_peer_pkt_test(cpu6, slv3, 0x01ac, 0x00ac, READ, pkt63);
    peer_to_peer_pkt_test(cpu7, slv3, 0x01fc, 0x00fc, READ, pkt73);

    peer_to_peer_pkt_test(cpu4, slv4, 0x3100, 0x0000, READ, pkt44);
    peer_to_peer_pkt_test(cpu5, slv4, 0x3154, 0x0054, READ, pkt54);
    peer_to_peer_pkt_test(cpu6, slv4, 0x32ac, 0x01ac, READ, pkt64);
    peer_to_peer_pkt_test(cpu7, slv4, 0x32fc, 0x01fc, READ, pkt74);

    peer_to_peer_pkt_test(cpu4, slv5, 0x3300, 0x0000, READ, pkt45);
    peer_to_peer_pkt_test(cpu5, slv5, 0x3854, 0x0554, READ, pkt55);
    peer_to_peer_pkt_test(cpu6, slv5, 0x3aac, 0x07ac, READ, pkt65);
    peer_to_peer_pkt_test(cpu7, slv5, 0x3ffc, 0x0cfc, READ, pkt75);

    peer_to_peer_pkt_test(cpu4, slv6, 0x4000, 0x0000, READ, pkt46);
    peer_to_peer_pkt_test(cpu5, slv6, 0x4854, 0x0854, READ, pkt56);
    peer_to_peer_pkt_test(cpu6, slv6, 0x5aac, 0x1aac, READ, pkt66);
    peer_to_peer_pkt_test(cpu7, slv6, 0x5ffc, 0x1ffc, READ, pkt76);

    peer_to_peer_pkt_test(cpu4, slv7, 0x6000, 0x0000, READ, pkt47);
    peer_to_peer_pkt_test(cpu5, slv7, 0x6854, 0x0854, READ, pkt57);
    peer_to_peer_pkt_test(cpu6, slv7, 0x6aac, 0x0aac, READ, pkt67);
    peer_to_peer_pkt_test(cpu7, slv7, 0x6ffc, 0x0ffc, READ, pkt77);

    peer_to_peer_pkt_test(cpu4, slv8, 0x7000, 0x0000, READ, pkt48);
    peer_to_peer_pkt_test(cpu5, slv8, 0x7854, 0x0854, READ, pkt58);
    peer_to_peer_pkt_test(cpu6, slv8, 0x7aac, 0x0aac, READ, pkt68);
    peer_to_peer_pkt_test(cpu7, slv8, 0x7ffc, 0x0ffc, READ, pkt78);

    bad_address_test(cpu1, 0x0000, READ, pkt11);
    bad_address_test(cpu2, 0x8000, READ, pkt12);
    bad_address_test(cpu2, 0x7ffd, READ, pkt13);
    bad_address_test(cpu3, 0x9000, READ, pkt14);

    bad_address_test(cpu1, 0x20ff, READ, pkt21);
    bad_address_test(cpu1, 0x20fc, READ, pkt22);
    bad_address_test(cpu1, 0x20fd, READ, pkt23);
    bad_address_test(cpu2, 0x2300, READ, pkt24);
    bad_address_test(cpu2, 0x24fd, READ, pkt25);

    bad_address_test(cpu1, 0x30ff, READ, pkt31);
    bad_address_test(cpu3, 0x30fc, READ, pkt32);
    bad_address_test(cpu2, 0x3400, READ, pkt33);
    bad_address_test(cpu3, 0x35fd, READ, pkt34);
}

/*
CPU1 -> \
CPU2 -> |--> Switch1(
CPU3 -> /      0x100 :0x1000 --------------------------------------------> Slv1
               0x1000:0x2000 --------------------------------------------> Slv2
               0x2000:0x8000 ---> Switch2(
                              /     0x100 : 0x200 -----------------------> Slv3
                              |     0x3000:0x6000 -> Switch3(
CPU4 -> \                     |                        0x100 : 0x300 ----> Slv4
CPU5 -> |--> Switch4(         |                        0x300 :0x1000 ----> Slv5
CPU6 -> |      0x0   :0x6000 -/                        0x1000:0x3000 ----> Slv6
CPU7 -> /      0x6000:0x7000 --------------------------------------------> Slv7
               0x7000:0x8000 --------------------------------------------> Slv8
*/

