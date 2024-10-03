#include "asynk-executor-device/asynk-executor-device.hpp"
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/register-request-manager.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <mutex>

AsynkExecutorDevice::AsynkExecutorDevice(
    const InstructionRegistry &registry,
    RegisterRequestsManager &req_manager,
    std::mutex &m
) : registry(registry), req_manager(req_manager), m(m) {}

void AsynkExecutorDevice::add_instruction(BaseInstruction i) {
    std::unique_lock<std::mutex> lock(m);
    in_process.push_back(std::move(registry.parse_instruction(i)));
    GenericInstruction &ins = in_process.back();
    RegisterRequest &req = ins.get_request();
    req_manager.add_request(req);
}

void AsynkExecutorDevice::handle() {
    std::unique_lock<std::mutex> lock(m);
    auto it = std::ranges::find_if(in_process, [](const GenericInstruction &i) {
        return i.is_ready();
    });
    if (it == in_process.end()) return;
    GenericInstruction &i = *it;
    registry.get_functor(i)(i);
    req_manager.free_registers(i.get_request());
    in_process.erase(it);
}

bool AsynkExecutorDevice::has_instructions() const noexcept {
    std::unique_lock<std::mutex> lock(m);
    return !in_process.empty();
}

