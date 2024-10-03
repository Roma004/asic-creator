#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/packet-queue.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <modules-common/packet-sender.hpp>

PacketSender::PacketSender(PacketQueue &q) : q(q) {};

bool PacketSender::always(std::shared_ptr<PacketInterface> pkt) { return true; }

bool PacketSender::recv(std::shared_ptr<PacketInterface> &pkt, cond_t recv_if) {
    return q.pop_if(pkt, recv_if);
}
