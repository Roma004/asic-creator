#include "tri-register-container.hpp"
#include "tristate.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>

using TRC = TriRegisterContainer;

constexpr const TRC::reference TRC::get_ref(int idx) const noexcept {
    return reference(chunks, idx);
}

void TRC::set_chunk(size_t ch_idx, Tristate st) noexcept {
    chunks[ch_idx].fill(st);
}

void TRC::set(int idx, const Tristate &state) noexcept {
    chunks[idx / 4].set(idx % 4, state);
}

void TRC::resize_unsafe(size_t new_size) {
    RegChunk *chunks_copy = new RegChunk[new_size] {};
    if (chunks != nullptr) {
        std::copy(
            chunks, chunks + std::min(new_size, nchunks), chunks_copy
        );
        delete[] chunks;
    }
    chunks = chunks_copy;
}

TRC::TriRegisterContainer() : chunks(nullptr), nchunks(0), nsigns(0) {}
TRC::TriRegisterContainer(size_t nsigns, Tristate def_st) {
    this->nchunks = CEIL_DIV(nsigns, 4);
    this->nsigns = nsigns;
    if (nsigns == 0) chunks = nullptr;
    else chunks = new RegChunk[nchunks] {};
    for (int i = 0; i < nchunks; ++i)
        set_chunk(i, def_st);
}
TRC::TriRegisterContainer(const TriRegisterContainer &c) {
    nchunks = c.nchunks;
    nsigns = c.nsigns;
    chunks = new RegChunk[nchunks];
    std::copy(c.chunks, c.chunks + c.nchunks, chunks);
}
TRC::TriRegisterContainer(TriRegisterContainer &&c) {
    nchunks = c.nchunks;
    nsigns = c.nsigns;
    chunks = c.chunks;
    c.chunks = nullptr;
}
TRC::~TriRegisterContainer() { delete[] chunks; }

const TRC::reference TRC::operator[](int idx) const {
    if (idx >= nsigns || idx < 0)
        throw std::out_of_range(std::format("bad index passed: {}", idx));
    return get_ref(idx);
}

TriRegisterContainer &TRC::operator=(const TriRegisterContainer &ct) noexcept {
    TriRegisterContainer res(ct);
    delete[] chunks;
    chunks = res.chunks;
    nchunks = res.nchunks;
    nsigns = res.nsigns;
    res.chunks = nullptr;
    return *this;
}

TriRegisterContainer &TRC::operator=(TriRegisterContainer &&ct) noexcept {
    delete[] chunks;
    chunks = ct.chunks;
    nchunks = ct.nchunks;
    nsigns = ct.nsigns;
    ct.chunks = nullptr;
    return *this;
}

TRC::iterator TRC::begin() noexcept { return iterator(get_ref(0), nsigns); }

TRC::iterator TRC::end() noexcept { return iterator(get_ref(nsigns), nsigns); }

TRC::const_iterator TRC::begin() const noexcept { return cbegin(); }

TRC::const_iterator TRC::end() const noexcept { return cend(); }

TRC::const_iterator TRC::cbegin() const noexcept {
    return const_iterator(get_ref(0), nsigns);
}

TRC::const_iterator TRC::cend() const noexcept {
    return const_iterator(get_ref(nsigns), nsigns);
}

TRC::reverse_iterator TRC::rbegin() noexcept {
    return reverse_iterator(get_ref(nsigns - 1), nsigns);
}

TRC::reverse_iterator TRC::rend() noexcept {
    return reverse_iterator(get_ref(-1), nsigns);
}

TRC::const_reverse_iterator TRC::rbegin() const noexcept { return crbegin(); }

TRC::const_reverse_iterator TRC::rend() const noexcept { return crend(); }

TRC::const_reverse_iterator TRC::crbegin() const noexcept {
    return const_reverse_iterator(get_ref(nsigns - 1), nsigns);
}

TRC::const_reverse_iterator TRC::crend() const noexcept {
    return const_reverse_iterator(get_ref(-1), nsigns);
}
