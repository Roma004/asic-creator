#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <memory>
#include <modules-common/packet-receiver.hpp>

PacketReceiver::PacketReceiver(PacketQueue &q) : q(q) {};

void PacketReceiver::send(std::shared_ptr<PacketInterface> pkt) { q.push(pkt); }
