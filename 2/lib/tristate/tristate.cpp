#include "tristate.hpp"

Tristate::Tristate(Tristate::STATE st) : st(st) {};

Tristate::Tristate() : Tristate(X) {}

Tristate::Tristate(char c) : st(char2st(c)) {}
Tristate::Tristate(int c) : st(int2st(c)) {}

Tristate &Tristate::operator=(const Tristate &o) noexcept {
    st = o.st;
    return *this;
}

Tristate &Tristate::operator=(Tristate::STATE state) noexcept {
    st = state;
    return *this;
}

Tristate &Tristate::operator=(int state) noexcept {
    st = int2st(state);
    return *this;
}

Tristate &Tristate::operator=(char state) noexcept {
    st = char2st(state);
    return *this;
}

Tristate Tristate::operator~() const noexcept {
    if (*this == X) return X;
    return *this == F ? T : F;
}

Tristate Tristate::operator|(const Tristate &o) const noexcept {
    if (*this == T || o == T) return T;
    if (*this == X || o == X) return X;
    return F;
}

Tristate Tristate::operator&(const Tristate &o) const noexcept {
    if (*this == F || o == F) return F;
    if (*this == X || o == X) return X;
    return T;
}

Tristate Tristate::operator!() const noexcept { return ~*this; }

Tristate Tristate::operator||(const Tristate &o) const noexcept {
    return *this | o;
}

Tristate Tristate::operator&&(const Tristate &o) const noexcept {
    return *this & o;
}

Tristate Tristate::operator|(STATE o) const noexcept {
    return *this | Tristate(o);
}
Tristate Tristate::operator&(STATE o) const noexcept {
    return *this & Tristate(o);
}
Tristate Tristate::operator||(STATE o) const noexcept {
    return *this | Tristate(o);
}
Tristate Tristate::operator&&(STATE o) const noexcept {
    return *this & Tristate(o);
}
