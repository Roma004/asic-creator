#include <asic-engine/data-master/switch-context.hpp>
#include <stdexcept>

SwitchContext::SwitchContext() : data(0) {}

void SwitchContext::push_switch(uint8_t swc_id) {
    int idx = get_top_idx();
    if (idx == 8) throw std::runtime_error("Max switch enclosure excided");
    swcs[idx] = swc_id;
}

void SwitchContext::pop_switch(uint8_t swc_id) {
    int idx = get_top_idx();
    if (idx == 0)
        throw std::runtime_error("attempt to pop from empty shitch sequence");
    if (swcs[idx - 1] != swc_id)
        throw std::runtime_error("attempt to pop with incorrect switch id");
    swcs[idx - 1] = 0;
}

uint8_t SwitchContext::get_top_id() const noexcept {
    int idx = get_top_idx();
    return idx ? swcs[idx - 1] : 0;
}

int SwitchContext::get_top_idx() const noexcept {
    for (int i = 0; i < 8; ++i) {
        if (swcs[i] == 0) return i;
    }
    return 8;
}

