#include "tri-register-container.hpp"

using TRC = TriRegisterContainer;

constexpr inline uint8_t TRC::RegChunk::st2val(st state) noexcept {
    switch (state) {
    case st::F: return 0b00;
    case st::T: return 0b01;
    default: return 0b11;
    }
}

constexpr inline TRC::st TRC::RegChunk::val2st(uint8_t val) noexcept {
    switch (val) {
    case 0b00: return st::F;
    case 0b01: return st::T;
    default: return st::X;
    }
}

Tristate TRC::RegChunk::get(int idx) const noexcept {
    return val2st((data & (0b11 << (idx * 2))) >> (idx * 2));
}

void TRC::RegChunk::set(int idx, Tristate state) noexcept {
    uint8_t val = st2val(state);
    data &= ~(0b11 << (idx * 2));
    data |= (val << (idx * 2));
}

void TRC::RegChunk::fill(Tristate state) noexcept {
    uint8_t val = st2val(state);
    data = (val << 6) | (val << 4) | (val << 2) | (val << 0);
}
