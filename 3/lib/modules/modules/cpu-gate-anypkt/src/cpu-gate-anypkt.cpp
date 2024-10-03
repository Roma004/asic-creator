#include "cpu-gate-anypkt/cpu-gate-anypkt.hpp"
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/common.hpp>
#include <memory>

CPUGate::CPUGate(Sender &slave_sender, Receiver &slave_receiver) :
    slave_receiver(slave_receiver), slave_sender(slave_sender) {}

void CPUGate::send_request_pkt(std::shared_ptr<PacketInterface> pkt) noexcept {
    if (pkt->get_type() == READ) address = pkt->get_addr();
    slave_receiver.send(pkt);
}

bool CPUGate::recv_response_pkt(std::shared_ptr<PacketInterface> &pkt) noexcept {
    return slave_sender.recv(pkt, [this](std::shared_ptr<PacketInterface> pkt) {
        return pkt->get_addr() == address;
    });
}
