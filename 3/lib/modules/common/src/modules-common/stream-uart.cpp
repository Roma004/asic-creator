#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/common.hpp>
#include <asic-engine/handler-device.hpp>
#include <cstdio>
#include <cstring>
#include <istream>
#include <logger/logger.hpp>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <modules-common/interconect-packet.hpp>
#include <modules-common/oops-data.hpp>
#include <modules-common/packet-data.hpp>
#include <modules-common/stream-uart.hpp>
#include <ostream>

StreamUART::StreamUART(DeviceGate &gate, std::istream &in, std::ostream &out) :
    gate(gate), in(in), out(out) {}

void StreamUART::send_oops(std::shared_ptr<PacketInterface> old_pkt) noexcept {
    auto data = std::make_shared<OOPSData>();
    auto pkt = std::make_shared<InterconectPacket>(*old_pkt, data);
    gate.send_response_pkt(pkt);
}

bool StreamUART::is_in_ready() const {
    return bool(in) && !in.eof() && !in.fail();
}

void StreamUART::send_resp(
    std::shared_ptr<PacketInterface> req_pkt, uint32_t val
) {
    auto data = std::make_shared<PacketDataX32>(val);
    auto pkt = std::make_shared<InterconectPacket>(*req_pkt, data);
    gate.send_response_pkt(pkt);
}

void StreamUART::send_csr(std::shared_ptr<PacketInterface> req_pkt) {
    csr.rx_empty = is_in_ready();
    csr.tx_empty = 1;
    send_resp(req_pkt, *(uint32_t *)&csr);
}

void StreamUART::handle_write(std::shared_ptr<PacketInterface> pkt) {
    if (pkt->get_addr() != 4) return;
    if (pkt->get_data().type_id() != "_X32") return;

    char c =
        (char)dynamic_cast<const PacketDataX32 &>(pkt->get_data()).get_data();

    out << c;
    if (out.fail() || out.bad()) {
        csr.tx_error = 1;
        return;
    }
    out.flush();
}

void StreamUART::send_input(std::shared_ptr<PacketInterface> req_pkt) {
    if (!is_in_ready()) {
        csr.rx_error = 1;
        send_resp(req_pkt, 0);
        return;
    }
    char res = in.get();

    send_resp(req_pkt, res);
}

void StreamUART::handle_read(std::shared_ptr<PacketInterface> pkt) {
    switch (pkt->get_addr()) {
    case 0x00: send_csr(pkt); break;
    case 0x08: send_input(pkt); break;
    default: send_oops(pkt);
    }
}

void StreamUART::handle() {
    std::shared_ptr<PacketInterface> pkt;
    if (!gate.recv_request_pkt(pkt)) return;

    if (pkt->get_type() == WRITE) handle_write(pkt);
    else if (pkt->get_type() == READ) handle_read(pkt);
}
