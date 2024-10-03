#pragma once
#include <cstdint>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/transfer-interface.hpp>
#include <asic-engine/data-master/common.hpp>
#include <modules-common/device-gate.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <functional>
#include <initializer_list>
#include <memory>
#include <vector>

class Switch : public SwitchInterface {
  public:
    Switch(
        uint8_t swc_id,
        PacketQueue &in_q,
        PacketQueue &out_q,
        SlaveInterface &def_slv
    );

    Receiver &get_receiver() noexcept override;
    Sender &get_sender() noexcept override;

    void transfer() override;

    void connect_slaves(
        const std::initializer_list<std::pair<AddrRange, SlaveInterface &>> &l
    ) override;

    void connect_slave(AddrRange rng, SlaveInterface &slv) override;

  private:
    struct range {
        AddrRange rng;
        std::reference_wrapper<SlaveInterface> slv;
    };

    std::vector<range> slaves;
    SlaveInterface &default_slave;
    DeviceGate gate;
    uint8_t swc_id;

    void handle_range(const range &rng) noexcept;

    bool
    try_extract_pkt(SlaveInterface &slv, std::shared_ptr<PacketInterface> &pkt);

    void transfer_pkt_to_master(
        std::shared_ptr<PacketInterface> pkt, addr_t addr_ext
    ) noexcept;

    std::vector<range>::const_iterator
    get_range(std::shared_ptr<PacketInterface> pkt) const noexcept;
};


