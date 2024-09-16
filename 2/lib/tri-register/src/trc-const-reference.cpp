#include "tri-register-container.hpp"

using TRC = TriRegisterContainer;
using cref = TRC::const_reference;

cref::const_reference(const reference &ref) :
    chunks(ref.chunks), idx(ref.idx) {}

cref::operator Tristate() const noexcept { return (st)(*this); }

cref::operator st() const noexcept { return chunks[idx / 4].get(idx % 4); }

bool cref::operator==(const const_reference &o) const noexcept {
    return (Tristate)(*this) == (Tristate)o;
}

bool cref::operator!=(const const_reference &o) const noexcept {
    return !(*this == o);
}
