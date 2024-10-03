#pragma once
#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/packet-queue.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>

class PacketSender : public Sender {
  public:
    PacketSender(PacketQueue &q);

    static bool always(std::shared_ptr<PacketInterface> pkt);

    bool recv(std::shared_ptr<PacketInterface> &pkt, cond_t recv_if = always)
        override;

  private:
    PacketQueue &q;
};
