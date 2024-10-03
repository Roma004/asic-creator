#pragma once
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <modules-common/device-gate.hpp>
#include <fstream>
#include <asic-engine/handler-device.hpp>
#include <memory>

class FileTcm : public PeripheralInterface {
    std::fstream s;
    DeviceGate &gate;
    size_t max_size;

  public:
    FileTcm(const char *filename, size_t max_size, DeviceGate &gate);

    void send_oops(std::shared_ptr<PacketInterface> old_pkt) noexcept;

    void handle_write(std::shared_ptr<PacketInterface> pkt);
    void handle_read(std::shared_ptr<PacketInterface> pkt);
    void handle() override;
};

