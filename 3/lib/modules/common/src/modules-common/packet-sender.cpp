#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/packet-queue.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>
#include <modules-common/packet-sender.hpp>
#include <optional>

PacketSender::PacketSender(PacketQueue &q) : q(q) {};

bool PacketSender::always(std::shared_ptr<PacketInterface> pkt) { return true; }

std::optional<std::shared_ptr<PacketInterface>>
PacketSender::recv(cond_t recv_if) {
    return q.pop_if(recv_if);
}
