#pragma once
#include <cstddef>
#include <cstdint>
#include <data-master/abstracts/abstract-packet.hpp>
#include <data-master/common.hpp>
#include <data-master/engine/interconect-packet.hpp>
#include <memory>

inline int test_packet_data_counter = 0;

class TestPacketData : public AbstractPacket::PacketData {
  public:
    constexpr const static std::string pkt_type = "test";
    uint32_t data;

    TestPacketData(uint32_t data) : data(data) {
        test_packet_data_counter += 1;
    }
    TestPacketData(const TestPacketData &o) : data(o.data) {
        test_packet_data_counter += 1;
    }
    TestPacketData(TestPacketData &&o) : data(o.data) {
        test_packet_data_counter += 1;
    }
    ~TestPacketData() override { test_packet_data_counter -= 1; }

    constexpr size_t sizeof_data() const noexcept override {
        return sizeof(uint32_t);
    }

    constexpr const std::string &type_id() const noexcept override {
        return pkt_type;
    }

    bool operator==(const PacketData &o) const noexcept override {
        if (o.type_id() != pkt_type) return false;
        auto pkt = dynamic_cast<const TestPacketData &>(o);
        return data == pkt.data;
    }
};

static inline std::
    pair<std::shared_ptr<InterconectPacket>, std::shared_ptr<TestPacketData>>
    make_packet(OPType type, addr_t addr, uint32_t data) {
    auto data_ptr = std::make_shared<TestPacketData>(data);
    auto pkt = std::make_shared<InterconectPacket>(type, addr, data_ptr);
    return {pkt, data_ptr};
}

static inline std::
    pair<std::shared_ptr<InterconectPacket>, std::shared_ptr<TestPacketData>>
    make_packet(std::shared_ptr<AbstractPacket> old_pkt, uint32_t data) {
    auto data_ptr = std::make_shared<TestPacketData>(data);
    auto pkt = std::make_shared<InterconectPacket>(*old_pkt, data_ptr);
    return {pkt, data_ptr};
}
