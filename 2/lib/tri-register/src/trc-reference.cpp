#include "tri-register-container.hpp"

using TRC = TriRegisterContainer;
using ref = TRC::reference;

ref::operator Tristate() const noexcept { return (st)(*this); }

ref::operator st() const noexcept { return chunks[idx / 4].get(idx % 4); }

const TRC::reference &ref::operator=(const Tristate &state) const noexcept {
    chunks[idx / 4].set(idx % 4, state);
    return *this;
}

bool ref::operator==(const reference &o) const noexcept {
    return (Tristate)(*this) == (Tristate)o;
}

bool ref::operator!=(const reference &o) const noexcept {
    return !(*this == o);
}
