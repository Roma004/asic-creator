#include "filetcm/filetcm.hpp"
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/common.hpp>
#include <cassert>
#include <cstring>
#include <fstream>
#include <logger/logger.hpp>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <modules-common/interconect-packet.hpp>
#include <modules-common/oops-data.hpp>
#include <modules-common/packet-data.hpp>
#include <stdexcept>

FileTcm::FileTcm(const char *filename, size_t max_size, DeviceGate &gate) :
    gate(gate), max_size(max_size) {
    s.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!s.is_open())
        throw std::runtime_error(std::format(
            "Unable to open file `{}`: {}", filename, strerror(errno)
        ));
}

void FileTcm::send_oops(std::shared_ptr<PacketInterface> old_pkt) noexcept {
    auto data = std::make_shared<OOPSData>();
    auto pkt = std::make_shared<InterconectPacket>(*old_pkt, data);
    gate.send_response_pkt(pkt);
}

void FileTcm::handle_write(std::shared_ptr<PacketInterface> pkt) {
    if (pkt->get_addr() > max_size) { return; }
    uint64_t buf = 0;
    if (pkt->get_data().type_id() == "_X8")
        buf = dynamic_cast<const PacketDataX8 &>(pkt->get_data()).get_data();
    else if (pkt->get_data().type_id() == "_X16")
        buf = dynamic_cast<const PacketDataX16 &>(pkt->get_data()).get_data();
    else if (pkt->get_data().type_id() == "_X32")
        buf = dynamic_cast<const PacketDataX32 &>(pkt->get_data()).get_data();
    else if (pkt->get_data().type_id() == "_X64")
        buf = dynamic_cast<const PacketDataX64 &>(pkt->get_data()).get_data();
    else return;
    debug.write("tcm data to write: {:x}", buf);
    s.seekp(pkt->get_addr(), std::ios::beg);
    s.write((const char *)&buf, pkt->get_data().sizeof_data());
}

void FileTcm::handle_read(std::shared_ptr<PacketInterface> pkt) {
    if (pkt->get_addr() > max_size) {
        debug.write("tcm::handle_read got bad addr {:x}", pkt->get_addr());
        send_oops(pkt);
        return;
    }
    uint64_t buf = 0;
    s.seekg(pkt->get_addr(), std::ios::beg);
    s.read((char *)&buf, pkt->get_data().sizeof_data());

    debug.write("tcm got data: {:x}", buf);

    std::shared_ptr<PacketDataInterface> new_data;
    if (pkt->get_data().type_id() == "_X8")
        new_data = std::make_shared<PacketDataX8>((uint8_t)buf);
    else if (pkt->get_data().type_id() == "_X16")
        new_data = std::make_shared<PacketDataX16>((uint16_t)buf);
    else if (pkt->get_data().type_id() == "_X32")
        new_data = std::make_shared<PacketDataX32>((uint32_t)buf);
    else if (pkt->get_data().type_id() == "_X64")
        new_data = std::make_shared<PacketDataX64>((uint64_t)buf);
    else {
        send_oops(pkt);
        return;
    }

    auto new_pkt = std::make_shared<InterconectPacket>(*pkt, new_data);
    debug.write(
        "tcm response to send: {}, {:x}, {:x}",
        (new_pkt->get_type() == READ ? "READ" : "WRITE"),
        new_pkt->get_addr(),
        buf
    );
    gate.send_response_pkt(new_pkt);
}

void FileTcm::handle() {
    std::shared_ptr<PacketInterface> pkt;
    if (!gate.recv_request_pkt(pkt)) return;
    debug.write(
        "tcm got request packet: {}, {:x}",
        (pkt->get_type() == READ ? "READ" : "WRITE"),
        pkt->get_addr()
    );

    if (pkt->get_type() == WRITE) handle_write(pkt);
    else if (pkt->get_type() == READ) handle_read(pkt);
}

