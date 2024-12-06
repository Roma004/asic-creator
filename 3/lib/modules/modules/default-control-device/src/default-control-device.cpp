#include "default-control-device.hpp"
#include <asic-engine/cpu/abstracts/data-master-port.hpp>
#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/handler-device.hpp>
#include <cstdint>
#include <logger/logger.hpp>

ControlDevice::ControlDevice(
    InstructionRegistry &i_registry,
    ExecutorDeviceInterface &control_executor,
    DataMasterPortInterface &port,
    uint32_t init_pc
) :
    i_reg(i_registry), port(port), pc(init_pc),
    control_executor(control_executor) {}

void ControlDevice::handle() {
    if (control_executor.has_instructions() || is_completed()) return;
    if (prev_pc == pc) pc += 4;
    prev_pc = pc;
    BaseInstruction i(load_instruction());
    debug.write("cpu processing instruction with opcode {:x}", i.get_opcode());
    i_reg.get_executor(i).add_instruction(i);
}

bool ControlDevice::is_completed() const noexcept { return pc == 0; }

std::atomic_uint32_t &ControlDevice::get_pc() noexcept { return pc; }
const std::atomic_uint32_t &ControlDevice::get_pc() const noexcept {
    return pc;
}

uint32_t ControlDevice::load_instruction() {
    uint32_t res = port.read32(pc);
    uint32_t pc_val = pc;
    debug.write("cpu got instruction `{:x}` at {:x}", res, pc_val);
    return res;
}

