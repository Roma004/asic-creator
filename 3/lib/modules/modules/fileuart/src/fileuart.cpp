#include "../include/fileuart/fileuart.hpp"
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/common.hpp>
#include <asic-engine/handler-device.hpp>
#include <cstring>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <modules-common/interconect-packet.hpp>
#include <modules-common/oops-data.hpp>
#include <modules-common/packet-data.hpp>
#include <ostream>

FileUART::FileUART(DeviceGate &gate, const std::string &filename) : gate(gate) {
    stream.open(filename);
    if (!stream.is_open())
        throw std::runtime_error(std::format(
            "Unable to open file `{}`: {}", filename, strerror(errno)
        ));
}

void FileUART::send_oops(std::shared_ptr<PacketInterface> old_pkt) noexcept {
    auto data = std::make_shared<OOPSData>();
    auto pkt = std::make_shared<InterconectPacket>(*old_pkt, data);
    gate.send_response_pkt(pkt);
}

void FileUART::send_resp(
    std::shared_ptr<PacketInterface> req_pkt, uint32_t val
) {
    auto data = std::make_shared<PacketDataX32>(val);
    auto pkt = std::make_shared<InterconectPacket>(*req_pkt, data);
    gate.send_response_pkt(pkt);
}

void FileUART::send_csr(std::shared_ptr<PacketInterface> req_pkt) {
    send_resp(req_pkt, *(uint32_t *)&csr);
}

void FileUART::handle_write(std::shared_ptr<PacketInterface> pkt) {
    if (pkt->get_addr() != 4) return;
    if (pkt->get_data().type_id() != "_X32") return;

    char c =
        (char)dynamic_cast<const PacketDataX32 &>(pkt->get_data()).get_data();

    stream << c;
}

void FileUART::handle_read(std::shared_ptr<PacketInterface> pkt) {
    switch (pkt->get_addr()) {
    case 0x00: send_csr(pkt); break;
    default: send_oops(pkt);
    }
}

void FileUART::handle() {
    if (auto opt = gate.recv_request_pkt()) {
        auto pkt = opt.value();
        if (pkt->get_type() == WRITE) handle_write(pkt);
        else if (pkt->get_type() == READ) handle_read(pkt);
    }
}
