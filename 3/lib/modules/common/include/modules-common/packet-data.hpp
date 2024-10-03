#pragma once
#include <concepts>
#include <asic-engine/data-master/abstracts/packet.hpp>

// TODO подумать, надо ли тут ещё что-то для типа T указывать
// TODO тесты под это дело
template <
    std::equality_comparable T,
    const char *TypeId,
    size_t TSize = sizeof(T)>
class PacketData : public PacketDataInterface {
  public:
    PacketData() = delete;
    explicit PacketData(const T &val) : data(val) {}
    explicit PacketData(T &&val) : data(std::move(val)) {}
    PacketData(const PacketData<T, TypeId, TSize> &) = default;
    PacketData(PacketData<T, TypeId, TSize> &&) = default;
    ~PacketData() override = default;

    constexpr const std::string &type_id() const noexcept override {
        return type_id_str;
    }

    constexpr size_t sizeof_data() const noexcept override { return TSize; };

    bool operator==(const PacketDataInterface &o
    ) const noexcept override {
        if (o.type_id() != type_id() || o.sizeof_data() != sizeof_data())
            return false;
        auto pkt = dynamic_cast<const PacketData<T, TypeId, TSize> &>(o);
        return pkt.data == data;
    }

    T &get_data() { return data; }
    const T &get_data() const { return data; }

  private:
    T data;
    constexpr static const std::string type_id_str = TypeId;
};

constexpr inline const char g_packet_data_x8_string[] = "_X8";
constexpr inline const char g_packet_data_x16_string[] = "_X16";
constexpr inline const char g_packet_data_x32_string[] = "_X32";
constexpr inline const char g_packet_data_x64_string[] = "_X64";

using PacketDataX8 = PacketData<uint8_t, g_packet_data_x8_string>;
using PacketDataX16 = PacketData<uint16_t, g_packet_data_x16_string>;
using PacketDataX32 = PacketData<uint32_t, g_packet_data_x32_string>;
using PacketDataX64 = PacketData<uint64_t, g_packet_data_x64_string>;
