#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <modules-common/packet-receiver.hpp>
#include <modules-common/packet-sender.hpp>

DeviceGate::DeviceGate(PacketQueue &mosi_q, PacketQueue &miso_q) :
    sender(mosi_q), receiver(miso_q), sender_inner(miso_q),
    receiver_inner(mosi_q) {}

Sender &DeviceGate::get_sender() noexcept { return sender; }
Receiver &DeviceGate::get_receiver() noexcept { return receiver; }

void DeviceGate::send_response_pkt(std::shared_ptr<PacketInterface> pkt) {
    receiver_inner.send(pkt);
}

bool DeviceGate::recv_request_pkt(std::shared_ptr<PacketInterface> &pkt) {
    return sender_inner.recv(pkt);
}
