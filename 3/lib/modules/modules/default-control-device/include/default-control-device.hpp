#pragma once

#include <asic-engine/cpu/abstracts/control-device.hpp>
#include <asic-engine/cpu/abstracts/data-master-port.hpp>
#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/handler-device.hpp>
#include <logger/logger.hpp>

class ControlDevice : public ControlDeviceInterface {
  public:
    ControlDevice(
        InstructionRegistry &i_registry,
        ExecutorDeviceInterface &control_executor,
        DataMasterPortInterface &port,
        uint32_t init_pc = 0x0
    );

    void handle() override;

    bool is_completed() const noexcept override;

    uint32_t &get_pc() noexcept override;
    const uint32_t &get_pc() const noexcept override;

  private:
    ExecutorDeviceInterface &control_executor;
    InstructionRegistry &i_reg;
    DataMasterPortInterface &port;
    uint32_t pc;
    uint32_t prev_pc = 0;

    uint32_t load_instruction();
};
