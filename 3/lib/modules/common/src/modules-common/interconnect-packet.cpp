#include <asic-engine/data-master/abstracts/packet.hpp>
#include <modules-common/interconect-packet.hpp>
#include <memory>

InterconectPacket::InterconectPacket(
    OPType type, addr_t addr, std::shared_ptr<PacketDataInterface> data
) : type(type), addr(addr), data(data), swc_ctx() {}

InterconectPacket::InterconectPacket(
    const PacketInterface &pkt, std::shared_ptr<PacketDataInterface> data
) :
    type(pkt.get_type()), addr(pkt.get_addr()), data(data),
    swc_ctx(pkt.get_switch_context()) {}

bool InterconectPacket::operator==(const PacketInterface &o) const noexcept {
    return type == o.get_type() && addr == o.get_addr()
        && *data == o.get_data();
};

bool InterconectPacket::operator==(const InterconectPacket &o) const noexcept {
    return *this == dynamic_cast<const PacketInterface &>(o);
};

OPType InterconectPacket::get_type() const noexcept { return type; }

addr_t InterconectPacket::get_addr() const noexcept { return addr; }

void InterconectPacket::set_addr(addr_t a) noexcept { addr = a; }

const PacketDataInterface &InterconectPacket::get_data() const noexcept {
    return *std::dynamic_pointer_cast<PacketDataInterface>(data);
}

const SwitchContext &InterconectPacket::get_switch_context() const noexcept {
    return swc_ctx;
}

SwitchContext &InterconectPacket::get_switch_context() noexcept {
    return swc_ctx;
}
