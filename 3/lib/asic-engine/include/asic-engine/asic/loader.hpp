#pragma once
#include "asic-engine/asic/configs.hpp"
#include "asic-engine/cpu/abstracts/control-device.hpp"
#include "asic-engine/cpu/abstracts/data-master-port.hpp"
#include "asic-engine/cpu/abstracts/executor-device.hpp"
#include "asic-engine/cpu/abstracts/instruction-form.hpp"
#include "asic-engine/cpu/abstracts/instruction-functor.hpp"
#include "asic-engine/cpu/abstracts/registers.hpp"
#include "asic-engine/cpu/registries/cpu-context.hpp"
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/data-master/abstracts/transfer-interface.hpp"
#include "asic-engine/data-master/bus-context.hpp"
#include "asic-engine/handler-device.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include "asic-engine/peripheral/peripheral-context.hpp"
#include "mod-loader/library-manger.hpp"
#include "mod-loader/module-manager.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

class ASICLoader {
  public:
    struct CPUHandlers {
        const std::string &name;
        HandlerDevice &control;
        std::vector<std::reference_wrapper<HandlerDevice>> executors;
    };

    struct PeripheralHandler {
        const std::string &name;
        HandlerDevice &peripheral;
    };

    struct TransferHandler {
        const std::string &name;
        TransferInterface &dev;
    };

    ASICLoader(
        const std::string &config_file,
        const std::string &mods_dir = "mods",
        const std::string &mod_ext = ".so"
    );

    std::vector<PeripheralHandler> &get_peripheral_handlers() noexcept;

    std::vector<CPUHandlers> &get_cpu_handlers() noexcept;

    std::vector<TransferHandler> &get_transfers() noexcept;

    bool cpus_stopped() const noexcept;

  private:
    void init_asic();
    void init_cpu(const std::string &name);

    void setup_cpu_handlers();

    ControlDeviceInterface &
    get_control_device(CPUConfig &cpu_cfg, CPUContext &cpu_ctx);

    RegisterBlock &get_registers(
        CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
    );

    InstructionForm &
    get_form(CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name);

    DataMasterPortInterface &
    get_port(CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name);

    ExecutorDeviceInterface &get_executor(
        CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
    );

    InstructionFunctor &get_functor(
        CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
    );

    RootInterface &get_bus_master(const std::string &name);

    SlaveInterface &get_bus_slave(const std::string &name);

    SlaveInterface &get_bridge(const std::string &name);

    SlaveInterface &get_default_dev(const std::string &name);

    SlaveInterface &get_switch(const std::string &name);

    SlaveInterface &get_endpoint(const std::string &name);

    PeripheralInterface &get_peripheral(const std::string &name);

    template <class T> const T &get_module(BaseConfig &c) {
        return dynamic_cast<const T &>(mods.get_module(c.id));
    }

    ASICConfig cfg;
    std::unordered_map<std::string, CPUContext> cpus;
    BusContext bus_ctx;
    PeripheralRegistry peripherals;
    LibraryManager libs;
    ModuleManager &mods;
    std::vector<CPUHandlers> cpu_handlers;
    std::vector<PeripheralHandler> peripheral_handlers;
    std::vector<TransferHandler> transfers;
    uint8_t current_switch_id = 1;
};
