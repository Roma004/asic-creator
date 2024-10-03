#include <asic-engine/cpu/register-set.hpp>
#include <asic-engine/exceptions.hpp>
#include <memory>
#include <vector>

RegisterSet::RegisterSet(std::initializer_list<std::shared_ptr<register_t>> l) :
    RegisterSet(l.begin(), l.end()) {}

RegisterSet::RegisterSet(
    std::shared_ptr<register_t> rd,
    std::initializer_list<std::shared_ptr<register_t>> l
) : RegisterSet(rd, l.begin(), l.end()) {}

RegisterSet::RegisterSet(std::shared_ptr<register_t> rd) : regs(1, rd) {}

std::shared_ptr<RegisterSet::register_t> RegisterSet::get_rd() {
    if (regs[0] == nullptr)
        throw RegisterSetExc("Destination (rd) register is not presented");
    return regs[0];
}

std::shared_ptr<RegisterSet::register_t> RegisterSet::get_rsn(size_t n) {
    if (n == 0)
        throw RegisterSetExc(
            "Operand registers numbers from 1 (zero index not permited)"
        );
    if (n >= regs.size())
        throw RegisterSetExc(
            std::format("Operand (rs{}) register is not presented", n)
        );
    return regs[n];
}

std::span<std::shared_ptr<RegisterSet::register_t>>
RegisterSet::registers() noexcept {
    if (!has_rd())
        return std::span<std::shared_ptr<register_t>>(
            regs.begin() + 1, regs.end()
        );
    return regs;
}
