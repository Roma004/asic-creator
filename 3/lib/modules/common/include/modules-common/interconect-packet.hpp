#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/common.hpp>
#include <memory>

class InterconectPacket : public PacketInterface {
  public:
    InterconectPacket(
        OPType type, addr_t addr, std::shared_ptr<PacketDataInterface> data
    );

    InterconectPacket(
        const PacketInterface &pkt, std::shared_ptr<PacketDataInterface> data
    );

    InterconectPacket(const InterconectPacket &) = delete;
    InterconectPacket(InterconectPacket &&) = default;
    ~InterconectPacket() override = default;

    OPType get_type() const noexcept override;

    addr_t get_addr() const noexcept override;

    void set_addr(addr_t addr) noexcept override;

    const PacketDataInterface &get_data() const noexcept override;

    SwitchContext &get_switch_context() noexcept override;
    const SwitchContext &get_switch_context() const noexcept override;

    bool operator==(const PacketInterface &o) const noexcept override;
    bool operator==(const InterconectPacket &o) const noexcept;

  private:
    OPType type;
    addr_t addr;
    std::shared_ptr<PacketDataInterface> data;
    SwitchContext swc_ctx;
};
