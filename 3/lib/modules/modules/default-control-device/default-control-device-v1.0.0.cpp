#include "asic-engine/cpu/modules/control-device-module.hpp"
#include "default-control-device.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"

class DefaultControlDeviceModule : public ControlDeviceModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "default-control-device", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<ControlDeviceInterface>
    make_control_device(CPUContext &ctx, ModuleSettings &stt) const override {
        auto &exec =
            ctx.executors.get_component(stt.get_string("control-executor"));
        auto &port = ctx.ports.get_component(stt.get_string("control-port"));
        return std::make_unique<ControlDevice>(
            ctx.instructions, exec, port, stt.get_int("pc")
        );
    }
};

DefaultControlDeviceModule default_control_device;

ModuleToken token(default_control_device);
