#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <memory>

class PacketReceiver : public Receiver {
  public:
    PacketReceiver(PacketQueue &q);

    void send(std::shared_ptr<PacketInterface> pkt);

  private:
    PacketQueue &q;
};
