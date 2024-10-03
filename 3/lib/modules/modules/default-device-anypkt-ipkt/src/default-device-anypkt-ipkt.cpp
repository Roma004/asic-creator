#include "default-device-anypkt-ipkt/default-device-anypkt-ipkt.hpp"
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/abstracts/transfer-interface.hpp>
#include <asic-engine/data-master/common.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <cstdio>
#include <memory>
#include <modules-common/interconect-packet.hpp>
#include <modules-common/packet-receiver.hpp>
#include <modules-common/packet-sender.hpp>

DefaultDevice::DefaultDevice(PacketQueue &in_q, PacketQueue &out_q) :
    gate(in_q, out_q) {}

Sender &DefaultDevice::get_sender() noexcept { return gate.get_sender(); }
Receiver &DefaultDevice::get_receiver() noexcept { return gate.get_receiver(); }

void DefaultDevice::transfer() {
    std::shared_ptr<PacketInterface> pkt;
    if (gate.recv_request_pkt(pkt)) {
        auto oops_pkt = std::make_shared<InterconectPacket>(
            *pkt, std::make_shared<OOPSData>()
        );
        gate.send_response_pkt(oops_pkt);
    }
}
