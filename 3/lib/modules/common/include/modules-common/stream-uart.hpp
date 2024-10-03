#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/peripheral/abstracts/peripheral.hpp>
#include <istream>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <ostream>

class StreamUART : public PeripheralInterface {
    struct uart_csr {
        uint32_t tx_empty : 1;
        uint32_t rx_empty : 1;
        uint32_t tx_error : 1;
        uint32_t rx_error : 1;
        uint32_t reserved0: 28;
    } __attribute__((packed, aligned(sizeof(uint32_t))));

    DeviceGate &gate;
    std::istream &in;
    std::ostream &out;
    uart_csr csr = {};

    bool is_in_ready() const;

    void send_oops(std::shared_ptr<PacketInterface> old_pkt) noexcept;
    void send_resp(std::shared_ptr<PacketInterface> req_pkt, uint32_t val);
    void send_csr(std::shared_ptr<PacketInterface> req_pkt);
    void send_input(std::shared_ptr<PacketInterface> req_pkt);

    void handle_write(std::shared_ptr<PacketInterface> pkt);
    void handle_read(std::shared_ptr<PacketInterface> pkt);

  public:
    StreamUART(DeviceGate &gate, std::istream &in, std::ostream &out);
    void handle() override;
};
