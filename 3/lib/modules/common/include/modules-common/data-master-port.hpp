#pragma once
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/common.hpp"
#include <asic-engine/cpu/abstracts/data-master-port.hpp>
#include <asic-engine/exceptions.hpp>
#include <cstdint>
#include <logger/logger.hpp>
#include <memory>
#include <modules-common/interconect-packet.hpp>
#include <modules-common/packet-data.hpp>
#include <type_traits>

template <typename T>
concept AnyPacketDataWithGetData =
    std::is_base_of<PacketDataInterface, T>::value && requires(T *pkt) {
        { pkt->get_data() };
    };

class DataMasterPort : public DataMasterPortInterface {
  public:
    DataMasterPort(RootInterface &gate) : gate(gate) {}

    template <AnyPacketDataWithGetData T> auto read(addr_t addr) {
        auto data = std::make_shared<T>(0);
        auto pkt = std::make_shared<InterconectPacket>(READ, addr, data);

        debug.write("port sends read pkt {:x}", addr);

        gate.send_request_pkt(std::move(pkt));

        std::optional<std::shared_ptr<PacketInterface>> opt;
        while (!(opt = gate.recv_response_pkt()));
        std::shared_ptr<PacketInterface> resp = opt.value();

        try {
            pkt = std::dynamic_pointer_cast<InterconectPacket>(resp);
            return dynamic_cast<const T &>(pkt->get_data()).get_data();
        } catch (const std::bad_cast &e) {
            throw DataMasterPortExc(
                std::format("Invalid read of size {}", data->sizeof_data())
            );
        }
    }

    template <AnyPacketDataWithGetData T>
    void write(addr_t addr, auto data_val) {
        auto data = std::make_shared<T>(data_val);
        auto pkt = std::make_shared<InterconectPacket>(WRITE, addr, data);
        gate.send_request_pkt(pkt);
    }

    uint8_t read8(addr_t addr) { return read<PacketDataX8>(addr); }
    uint16_t read16(addr_t addr) { return read<PacketDataX16>(addr); }
    uint32_t read32(addr_t addr) override { return read<PacketDataX32>(addr); }
    uint64_t read64(addr_t addr) { return read<PacketDataX64>(addr); }

    void write8(addr_t addr, uint8_t data) { write<PacketDataX8>(addr, data); }
    void write16(addr_t addr, uint16_t data) {
        write<PacketDataX16>(addr, data);
    }
    void write32(addr_t addr, uint32_t data) {
        write<PacketDataX32>(addr, data);
    }
    void write64(addr_t addr, uint64_t data) {
        write<PacketDataX64>(addr, data);
    }

  private:
    RootInterface &gate;
};

