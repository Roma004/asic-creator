#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/abstracts/transfer-interface.hpp>
#include <asic-engine/data-master/common.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <modules-common/device-gate.hpp>
#include <stdexcept>
#include <switch-anypkt/switch-anypkt.hpp>
#include <vector>

Switch::Switch(
    uint8_t swc_id,
    PacketQueue &in_q,
    PacketQueue &out_q,
    SlaveInterface &def_slv
) : gate(in_q, out_q), swc_id(swc_id), default_slave(def_slv) {}

Receiver &Switch::get_receiver() noexcept { return gate.get_receiver(); }
Sender &Switch::get_sender() noexcept { return gate.get_sender(); }

// TODO do something with bad address got from slave
void Switch::transfer() {
    if (auto opt = gate.recv_request_pkt()) {
        auto pkt = opt.value();
        auto it = get_range(pkt);
        pkt->get_switch_context().push_switch(swc_id);
        if (it == slaves.end()
            || pkt->get_addr() % pkt->get_data().sizeof_data() != 0)
            default_slave.get_receiver().send(pkt);
        else {
            pkt->set_addr(pkt->get_addr() - it->rng.addr);
            it->slv.get().get_receiver().send(pkt);
        }
    }
    for (auto &rng : slaves) handle_range(rng);
    if (auto opt = try_extract_pkt(default_slave))
        transfer_pkt_to_master(opt.value(), 0);
};

void Switch::connect_slaves(
    const std::initializer_list<std::pair<AddrRange, SlaveInterface &>> &l
) {
    for (auto &&[rng, slv] : l) { connect_slave(rng, slv); }
}

void Switch::connect_slave(AddrRange rng, SlaveInterface &slv) {
    if (rng.addr % sizeof(addr_t) != 0)
        throw std::runtime_error("range base address bust be modular 4");
    if (rng.offset % sizeof(addr_t) != 0)
        throw std::runtime_error("range length bust be modular 4");

    range new_r = {rng, slv};

    // если ни одного диапазона не подключено, подключить
    if (slaves.empty()) {
        slaves.push_back(std::move(new_r));
        return;
    }

    // находим, перед каким диапазном надо вставить новый
    auto it = std::ranges::find_if(slaves, [rng](const range &cur_rng) {
        return cur_rng.rng.addr >= rng.addr;
    });

    auto is_intersected_by = [](AddrRange a, AddrRange b) {
        return b.addr >= a.addr && a.addr + a.offset > b.addr;
    };

    // если надо вставить в конец
    if (it == slaves.end()) {
        // если пересекаются с послес последним - ошибка
        if (is_intersected_by(slaves.back().rng, rng))
            throw std::runtime_error("ranges must not intersect each other");
        slaves.push_back(std::move(new_r));
        return;
    }

    // если пересекатся со следующим, ошибка
    if (is_intersected_by(rng, it->rng))
        throw std::runtime_error("ranges must not intersect each other");

    if (it == slaves.begin()) {
        slaves.insert(it, std::move(new_r));
        return;
    }

    auto prev = it - 1;
    if (is_intersected_by(prev->rng, rng))
        throw std::runtime_error("ranges must not intersect each other");
    slaves.insert(prev, std::move(new_r));
}

void Switch::handle_range(const range &rng) noexcept {
    if (auto opt = try_extract_pkt(rng.slv.get()))
        transfer_pkt_to_master(opt.value(), rng.rng.addr);
}

std::optional<std::shared_ptr<PacketInterface>>
Switch::try_extract_pkt(SlaveInterface &slv) {
    return slv.get_sender().recv([this](std::shared_ptr<PacketInterface> pkt) {
        return pkt->get_switch_context().get_top_id() == swc_id;
    });
}

void Switch::transfer_pkt_to_master(
    std::shared_ptr<PacketInterface> pkt, addr_t addr_ext
) noexcept {
    pkt->set_addr(pkt->get_addr() + addr_ext);
    pkt->get_switch_context().pop_switch(swc_id);
    gate.send_response_pkt(pkt);
}

std::vector<Switch::range>::const_iterator
Switch::get_range(std::shared_ptr<PacketInterface> pkt) const noexcept {
    return std::ranges::find_if(slaves, [&pkt](const range &rng) {
        return rng.rng.addr <= pkt->get_addr()
            && pkt->get_addr() <= rng.rng.addr + rng.rng.offset
                                      - pkt->get_data().sizeof_data();
    });
}

