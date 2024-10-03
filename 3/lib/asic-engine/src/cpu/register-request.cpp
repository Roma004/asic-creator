#include <algorithm>
#include <asic-engine/cpu/register-request.hpp>
#include <asic-engine/cpu/register-set.hpp>
#include <memory>

RegisterRequest::RegisterRequest(RegisterSet &regs) : regs(regs) {};

bool RegisterRequest::is_ready() const noexcept {
    return std::ranges::all_of(
        regs.registers(),
        [](const std::shared_ptr<register_t> &reg) {
            return reg->is_borrowed();
        }
    );
}

std::span<std::shared_ptr<RegisterRequest::register_t>>
RegisterRequest::registers() noexcept {
    return regs.registers();
}

bool RegisterRequest::is_register_requested(unsigned int reg_id
) const noexcept {
    return std::ranges::find_if(
               regs.registers(),
               [reg_id](const std::shared_ptr<register_t> &reg) {
                   return reg->get_register_id() == reg_id;
               }
           )
        != regs.registers().end();
}

