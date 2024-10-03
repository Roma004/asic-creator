#include "synk-executor-device/synk-executor-device.hpp"
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/register-request-manager.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/handler-device.hpp>
#include <logger/logger.hpp>

SynkExecutorDevice::SynkExecutorDevice(
    const InstructionRegistry &registry,
    RegisterRequestsManager &req_manager,
    std::mutex &m
) : registry(registry), req_manager(req_manager), m(m) {}

void SynkExecutorDevice::add_instruction(BaseInstruction i) {
    std::unique_lock<std::mutex> lock(m);
    in_process.push(std::move(registry.parse_instruction(i)));
    GenericInstruction &ins = in_process.back();
    RegisterRequest &req = ins.get_request();
    req_manager.add_request(req);
}

void SynkExecutorDevice::handle() {
    if (in_process.empty()) return;
    std::unique_lock<std::mutex> lock(m);
    GenericInstruction &i = in_process.front();
    if (!i.is_ready()) return;
    debug.write(
        "Processing {}-instruction: {:x}-{:x}-{:x}",
        i.get_form().get_form(),
        i.get_opcode(),
        i.get_minor(),
        i.get_sub_minor()
    );
    registry.get_functor(i)(i);
    req_manager.free_registers(i.get_request());
    in_process.pop();
}

bool SynkExecutorDevice::has_instructions() const noexcept {
    std::unique_lock<std::mutex> lock(m);
    return !in_process.empty();
}
