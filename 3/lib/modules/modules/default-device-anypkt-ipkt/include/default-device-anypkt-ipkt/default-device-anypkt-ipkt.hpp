#pragma once
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <cstdio>
#include <modules-common/device-gate.hpp>
#include <modules-common/oops-data.hpp>
#include <modules-common/packet-receiver.hpp>
#include <modules-common/packet-sender.hpp>

class DefaultDevice : public DefaultDeviceInterface {
  public:
    DefaultDevice(PacketQueue &in_q, PacketQueue &out_q);

    Sender &get_sender() noexcept override;
    Receiver &get_receiver() noexcept override;

    void transfer() override;

  private:
    DeviceGate gate;
};
