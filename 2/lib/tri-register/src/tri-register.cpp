#include "tri-register.hpp"
#include "tri-register-container.hpp"
#include "tristate.hpp"
#include <algorithm>
#include <cstring>
#include <ranges>
#include <tuple>

TriRegister::reg_length_error::reg_length_error(
    const TriRegister &a, const TriRegister &b, const char *op
) noexcept :
    msg(std::format(
        "Binary operation `{}` between reqisters of lengths {} and {} is not a "
        "valid operation",
        op,
        a.size(),
        b.size()
    )) {}

char const *TriRegister::reg_length_error::what() const noexcept {
    return msg.c_str();
}

TriRegister::TriRegister(size_t nsigns, Tristate st) noexcept :
    signals(nsigns, st) {}

TriRegister::TriRegister(const auto *sgn, size_t n) noexcept :
    TriRegister(sgn, sgn + n) {}

TriRegister::TriRegister(const char *str) noexcept :
    TriRegister(str, str + strlen(str)) {}

TriRegister::reference TriRegister::operator[](int idx) { return signals[idx]; }

TriRegister TriRegister::operator|(const TriRegister &other) const {
    if (size() != other.size()) throw reg_length_error(*this, other, "|");
    auto applied = std::views::zip_transform(
        [](const Tristate &a, const Tristate &b) { return a | b; }, *this, other
    );
    return TriRegister(applied.begin(), applied.end());
}

TriRegister TriRegister::operator&(const TriRegister &other) const {
    if (size() != other.size()) throw reg_length_error(*this, other, "&");
    auto applied = std::views::zip_transform(
        [](const Tristate &a, const Tristate &b) { return a & b; }, *this, other
    );
    return TriRegister(applied.begin(), applied.end());
}

TriRegister TriRegister::operator~() const noexcept {
    auto applied =
        std::views::transform(*this, [](const Tristate &st) { return ~st; });
    return TriRegister(applied.begin(), applied.end());
}

TriRegister &TriRegister::operator|=(const TriRegister &other) {
    using namespace std::ranges::views;
    if (size() != other.size()) throw reg_length_error(*this, other, "|=");
    auto applied = zip_transform(
        [](const Tristate &a, const Tristate &b) { return a | b; }, *this, other
    );
    for (auto &&[a, b] : zip(*this, applied)) a = b;
    return *this;
}

TriRegister &TriRegister::operator&=(const TriRegister &other) {
    using namespace std::ranges::views;
    if (size() != other.size()) throw reg_length_error(*this, other, "&=");
    auto applied = zip_transform(
        [](const Tristate &a, const Tristate &b) { return a & b; }, *this, other
    );
    for (auto &&[a, b] : zip(*this, applied)) a = b;
    return *this;
}

TriRegister &TriRegister::operator=(const TriRegister &other) noexcept {
    signals = other.signals;
    return *this;
}

bool TriRegister::operator==(const TriRegister &other) const noexcept {
    if (size() != other.size()) return false;
    for (auto &&[a, b] : std::views::zip(*this, other))
        if (a != b) return false;
    return true;
}

bool TriRegister::operator!=(const TriRegister &other) const noexcept {
    return !(*this == other);
}

TriRegister::iter TriRegister::begin() noexcept { return signals.begin(); }
TriRegister::iter TriRegister::end() noexcept { return signals.end(); }
TriRegister::citer TriRegister::begin() const noexcept {
    return signals.cbegin();
}
TriRegister::citer TriRegister::end() const noexcept { return signals.cend(); }
TriRegister::citer TriRegister::cbegin() const noexcept { return begin(); }
TriRegister::citer TriRegister::cend() const noexcept { return end(); }

TriRegister::riter TriRegister::rbegin() noexcept { return signals.rbegin(); }
TriRegister::riter TriRegister::rend() noexcept { return signals.rend(); }
TriRegister::rciter TriRegister::rbegin() const noexcept {
    return signals.crbegin();
}
TriRegister::rciter TriRegister::rend() const noexcept {
    return signals.crend();
}
TriRegister::rciter TriRegister::crbegin() const noexcept { return rbegin(); }
TriRegister::rciter TriRegister::crend() const noexcept { return rend(); }

bool TriRegister::is_fully_defined() const noexcept {
    return std::ranges::all_of(*this, [](auto &val) {
        return val != Tristate::X;
    });
}

void TriRegister::extend_left(const TriRegister &reg) noexcept {
    TriRegisterContainer new_signs(reg.size() + size());
    auto it = new_signs.begin();
    auto copy = [&it](const Tristate &s){ *it = s; ++it; };
    std::ranges::for_each(reg, copy);
    std::ranges::for_each(*this, copy);
    signals = std::move(new_signs);
}

void TriRegister::extend_right(const TriRegister &reg) noexcept {
    TriRegisterContainer new_signs(reg.size() + size());
    auto it = new_signs.begin();
    auto copy = [&it](const Tristate &s){ *it = s; ++it; };
    std::ranges::for_each(*this, copy);
    std::ranges::for_each(reg, copy);
    signals = std::move(new_signs);
}
