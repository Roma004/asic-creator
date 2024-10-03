#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>

class OOPSData : public PacketDataInterface {
    constexpr const static std::string oops = "OOPS";
    constexpr const std::string &type_id() const noexcept override {
        return oops;
    }

    constexpr size_t sizeof_data() const noexcept override { return 0; }

    constexpr bool operator==(const PacketDataInterface &data) const noexcept override {
        return data.type_id() == oops;
    }
};

