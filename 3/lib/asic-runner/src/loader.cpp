#include "asic-engine/cpu/abstracts/control-device.hpp"
#include "asic-engine/cpu/abstracts/data-master-port.hpp"
#include "asic-engine/cpu/abstracts/executor-device.hpp"
#include "asic-engine/cpu/abstracts/instruction-form.hpp"
#include "asic-engine/cpu/abstracts/instruction-functor.hpp"
#include "asic-engine/cpu/abstracts/registers.hpp"
#include "asic-engine/cpu/modules/control-device-module.hpp"
#include "asic-engine/cpu/modules/data-master-port-module.hpp"
#include "asic-engine/cpu/modules/executor-device-module.hpp"
#include "asic-engine/cpu/modules/instruction-form-module.hpp"
#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "asic-engine/cpu/modules/registers-module.hpp"
#include "asic-engine/cpu/register-request-manager.hpp"
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/data-master/modules/default-device-module.hpp"
#include "asic-engine/data-master/modules/device-gate-module.hpp"
#include "asic-engine/data-master/modules/master-gate-module.hpp"
#include "asic-engine/data-master/modules/switch-module.hpp"
#include "asic-engine/handler-device.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include "asic-engine/peripheral/modules/peripheral-module.hpp"
#include "mod-loader/library-manger.hpp"
#include "mod-loader/module-manager.hpp"
#include <asic-runner/configs.hpp>
#include <asic-runner/loader.hpp>
#include <asic-runner/util.hpp>
#include <asic-engine/cpu/registries/cpu-context.hpp>
#include <asic-engine/data-master/bus-context.hpp>
#include <asic-engine/data-master/modules/bridge-module.hpp>
#include <asic-engine/peripheral/peripheral-context.hpp>
#include <format>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

ASICLoader::ASICLoader(
    const std::string &config_file,
    const std::string &mods_dir,
    const std::string &mod_ext
) :
    cpus(), bus_ctx(), peripherals(),
    cfg(nlohmann::json::parse(std::ifstream(config_file))),
    mods(ModuleManager::get_instance()) {
    libs.scan(mods_dir, mod_ext);
    init_asic();
}

std::vector<ASICLoader::PeripheralHandler> &
ASICLoader::get_peripheral_handlers() noexcept {
    return peripheral_handlers;
}

std::vector<ASICLoader::CPUHandlers> &ASICLoader::get_cpu_handlers() noexcept {
    return cpu_handlers;
}

std::vector<ASICLoader::TransferHandler> &ASICLoader::get_transfers() noexcept {
    return transfers;
}

bool ASICLoader::cpus_stopped() const noexcept {
    for (auto &&[key, cpu] : cpus)
        if (!cpu.ctrl->is_completed()) return false;
    return true;
}

void ASICLoader::init_asic() {
    for (auto &&[key, val] : cfg.cpus) init_cpu(key);
    for (auto &&[key, val] : cfg.peripherals) get_peripheral(key);
}

void ASICLoader::init_cpu(const std::string &name) {
    static std::vector<std::unique_ptr<std::mutex>> mutexes;
    auto &m = mutexes.emplace_back(std::make_unique<std::mutex>());

    CPUConfig &cpu_cfg = cfg.cpus.at(name);
    auto &&[it, ok] = cpus.emplace(
        name,
        CPUContext(
            {.cpu_name = name, .req_manager = RegisterRequestsManager(*m)}
        )
    );
    CPUContext &cpu_ctx = it->second;

    for (auto &[key, val] : cpu_cfg.registers)
        get_registers(cpu_cfg, cpu_ctx, key);
    for (auto &[key, val] : cpu_cfg.ports) get_port(cpu_cfg, cpu_ctx, key);
    for (auto &[key, val] : cpu_cfg.executors)
        get_executor(cpu_cfg, cpu_ctx, key);

    get_control_device(cpu_cfg, cpu_ctx);
    setup_cpu_handlers();

    for (auto &[key, val] : cpu_cfg.forms) get_form(cpu_cfg, cpu_ctx, key);
    for (auto &[key, val] : cpu_cfg.functors)
        get_functor(cpu_cfg, cpu_ctx, key);

    for (auto &ins : cpu_cfg.instructions) {
        auto &funct = get_functor(cpu_cfg, cpu_ctx, ins.functor_name);
        auto &form = get_form(cpu_cfg, cpu_ctx, ins.form_name);
        auto &exec = get_executor(cpu_cfg, cpu_ctx, ins.executor_name);
        cpu_ctx.instructions.add_instruction(ins.opcode, form, funct, exec);
    }
}

void ASICLoader::setup_cpu_handlers() {
    for (auto &&[name, ctx] : cpus) {
        std::vector<std::reference_wrapper<HandlerDevice>> execs;
        for (auto &&[name, val] : cfg.cpus.at(name).executors)
            execs.push_back(std::ref(ctx.executors.get_component(name)));
        cpu_handlers.emplace_back(name, *ctx.ctrl, std::move(execs));
    }
}

ControlDeviceInterface &
ASICLoader::get_control_device(CPUConfig &cpu_cfg, CPUContext &cpu_ctx) {
    if (cpu_ctx.ctrl) return *cpu_ctx.ctrl;
    ControlDeviceConfig &c_cfg = cpu_cfg.ctrl;
    cpu_ctx.ctrl = get_module<ControlDeviceModule>(c_cfg).make_control_device(
        cpu_ctx, c_cfg.settings
    );
    return *cpu_ctx.ctrl;
}

RegisterBlock &ASICLoader::get_registers(
    CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
) {
    if (!cpu_cfg.registers.has_config(name))
        throw std::runtime_error(std::format(
            "No register with name `{}` registered for cpu `{}`",
            name,
            cpu_ctx.cpu_name
        ));
    if (cpu_ctx.register_blocks.has_component(name))
        return cpu_ctx.register_blocks.get_component(name);
    RegistersConfig &c_cfg = cpu_cfg.registers.at(name);
    cpu_ctx.register_blocks.add_component(
        name, get_module<RegistersModule>(c_cfg).make_registers(c_cfg.settings)
    );
    return cpu_ctx.register_blocks.get_component(name);
}

InstructionForm &ASICLoader::get_form(
    CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
) {
    if (!cpu_cfg.forms.has_config(name))
        throw std::runtime_error(std::format(
            "No form with name `{}` registered for cpu `{}`",
            name,
            cpu_ctx.cpu_name
        ));
    if (cpu_ctx.forms.has_component(name))
        return cpu_ctx.forms.get_component(name);
    FormsConfig &c_cfg = cpu_cfg.forms.at(name);
    cpu_ctx.forms.add_component(
        name,
        get_module<InstructionFormModule>(c_cfg).make_form(
            cpu_ctx, c_cfg.settings
        )
    );
    return cpu_ctx.forms.get_component(name);
}

DataMasterPortInterface &ASICLoader::get_port(
    CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
) {
    if (!cpu_cfg.ports.has_config(name))
        throw std::runtime_error(std::format(
            "No port with name `{}` registered for cpu `{}`",
            name,
            cpu_ctx.cpu_name
        ));
    if (cpu_ctx.ports.has_component(name))
        return cpu_ctx.ports.get_component(name);
    DataMasterPortConfig &c_cfg = cpu_cfg.ports.at(name);
    RootInterface &gate = get_bus_master(c_cfg.bus_master_name);
    cpu_ctx.ports.add_component(
        name,
        get_module<DataMasterPortModule>(c_cfg).make_port(gate, c_cfg.settings)
    );
    return cpu_ctx.ports.get_component(name);
}

ExecutorDeviceInterface &ASICLoader::get_executor(
    CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
) {
    if (!cpu_cfg.executors.has_config(name))
        throw std::runtime_error(std::format(
            "No executor with name `{}` registered for cpu `{}`",
            name,
            cpu_ctx.cpu_name
        ));
    if (cpu_ctx.executors.has_component(name))
        return cpu_ctx.executors.get_component(name);
    ExecutorsConfig &c_cfg = cpu_cfg.executors.at(name);
    cpu_ctx.executors.add_component(
        name,
        get_module<ExecutorDeviceModule>(c_cfg).make_executor_device(
            cpu_ctx, c_cfg.settings
        )
    );
    auto &res = cpu_ctx.executors.get_component(name);
    return res;
}

InstructionFunctor &ASICLoader::get_functor(
    CPUConfig &cpu_cfg, CPUContext &cpu_ctx, const std::string &name
) {
    if (!cpu_cfg.functors.has_config(name))
        throw std::runtime_error(std::format(
            "No functor with name `{}` registered for cpu `{}`",
            name,
            cpu_ctx.cpu_name
        ));
    if (cpu_ctx.functors.has_component(name))
        return cpu_ctx.functors.get_component(name);
    FunctorsConfig &c_cfg = cpu_cfg.functors.at(name);
    cpu_ctx.functors.add_component(
        name,
        get_module<InstructionFunctorModule>(c_cfg).make_functor(
            cpu_ctx, c_cfg.settings
        )
    );
    return cpu_ctx.functors.get_component(name);
}

RootInterface &ASICLoader::get_bus_master(const std::string &name) {
    if (!cfg.bus.masters.has_config(name))
        throw std::runtime_error(
            std::format("No bus master with name `{}` registered", name)
        );
    if (bus_ctx.masters.has_component(name))
        return bus_ctx.masters.get_component(name);
    BusMasterConfig &mst_cfg = cfg.bus.masters.at(name);
    SlaveInterface &slv = get_bus_slave(mst_cfg.slave_name);
    bus_ctx.masters.add_component(
        name,
        get_module<MasterGateModule>(mst_cfg).make_master_gate(
            slv, mst_cfg.settings
        )
    );
    return bus_ctx.masters.get_component(name);
}

SlaveInterface &ASICLoader::get_bus_slave(const std::string &name) {
    if (cfg.bus.bridges.has_config(name)) return get_bridge(name);
    if (cfg.bus.default_devs.has_config(name)) return get_default_dev(name);
    if (cfg.bus.endpoints.has_config(name)) return get_endpoint(name);
    if (cfg.bus.switches.has_config(name)) return get_switch(name);
    throw std::runtime_error(
        std::format("No slave with name {} registered", name)
    );
}

SlaveInterface &ASICLoader::get_bridge(const std::string &name) {
    if (!cfg.bus.bridges.has_config(name))
        throw std::runtime_error(
            std::format("No bridge with name `{}` registered", name)
        );
    if (bus_ctx.bridges.has_component(name))
        return bus_ctx.bridges.get_component(name);
    BusBridgeConfig &slv_cfg = cfg.bus.bridges.at(name);
    SlaveInterface &slv = get_bus_slave(slv_cfg.slave_name);
    bus_ctx.bridges.add_component(
        name,
        get_module<BridgeModule>(slv_cfg).make_bridge(slv, slv_cfg.settings)
    );
    auto &res = bus_ctx.bridges.get_component(name);
    transfers.emplace_back(name, std::ref(res));
    return res;
}

SlaveInterface &ASICLoader::get_default_dev(const std::string &name) {
    if (!cfg.bus.default_devs.has_config(name))
        throw std::runtime_error(
            std::format("No default device with name `{}` registered", name)
        );
    if (bus_ctx.default_devs.has_component(name))
        return bus_ctx.default_devs.get_component(name);
    BusDefaultDeviceConfig &slv_cfg = cfg.bus.default_devs.at(name);
    bus_ctx.default_devs.add_component(
        name,
        get_module<DefaultDeviceModule>(slv_cfg).make_default_device(
            slv_cfg.settings
        )
    );
    auto &res = bus_ctx.default_devs.get_component(name);
    transfers.emplace_back(name, std::ref(res));
    return res;
}

SlaveInterface &ASICLoader::get_switch(const std::string &name) {
    if (!cfg.bus.switches.has_config(name))
        throw std::runtime_error(
            std::format("No switch with name `{}` registered", name)
        );
    if (bus_ctx.switches.has_component(name))
        return bus_ctx.switches.get_component(name);
    BusSwitchConfig &slv_cfg = cfg.bus.switches.at(name);
    SlaveInterface &slv = get_default_dev(slv_cfg.default_slave_name);
    bus_ctx.switches.add_component(
        name,
        get_module<SwitchModule>(slv_cfg).make_switch(
            current_switch_id++, slv, slv_cfg.settings
        )
    );
    auto &swc = bus_ctx.switches.get_component(name);
    for (auto &c : slv_cfg.slaves) {
        swc.connect_slave({c.base, c.size}, get_bus_slave(c.slave_name));
    }
    transfers.emplace_back(name, std::ref(swc));
    return swc;
}

SlaveInterface &ASICLoader::get_endpoint(const std::string &name) {
    if (!cfg.bus.endpoints.has_config(name))
        throw std::runtime_error(
            std::format("No endpoint with name `{}` registered", name)
        );
    if (bus_ctx.endpoints.has_component(name))
        return bus_ctx.endpoints.get_component(name);
    BusEndpointConfig &slv_cfg = cfg.bus.endpoints.at(name);
    bus_ctx.endpoints.add_component(
        name,
        get_module<DeviceGateModule>(slv_cfg).make_device_gate(slv_cfg.settings)
    );
    return bus_ctx.endpoints.get_component(name);
}

PeripheralInterface &ASICLoader::get_peripheral(const std::string &name) {
    if (!cfg.peripherals.has_config(name))
        throw std::runtime_error(
            std::format("No peripheral with name `{}` registered", name)
        );
    if (peripherals.has_component(name)) return peripherals.get_component(name);
    PeripheralConfig &slv_cfg = cfg.peripherals.at(name);
    EndpointInterface &slv =
        dynamic_cast<EndpointInterface &>(get_endpoint(slv_cfg.bus_endpoint_name
        ));
    peripherals.add_component(
        name,
        get_module<PeripheralModule>(slv_cfg).make_peripheral(
            slv, slv_cfg.settings
        )
    );
    auto &res = peripherals.get_component(name);
    peripheral_handlers.emplace_back(name, std::ref(res));
    return res;
}
