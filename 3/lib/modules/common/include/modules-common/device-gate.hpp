#pragma once
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <memory>
#include <modules-common/packet-receiver.hpp>
#include <modules-common/packet-sender.hpp>

class DeviceGate : public EndpointInterface {
  public:
    DeviceGate(PacketQueue &in_q, PacketQueue &out_q);

    Sender &get_sender() noexcept override;
    Receiver &get_receiver() noexcept override;

    void send_response_pkt(std::shared_ptr<PacketInterface> pkt) override;

    std::optional<std::shared_ptr<PacketInterface>> recv_request_pkt() override;

  private:
    PacketSender sender;
    PacketReceiver receiver;

    PacketSender sender_inner;
    PacketReceiver receiver_inner;
};
