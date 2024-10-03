#include "asic-engine/cpu/modules/executor-device-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "synk-executor-device/synk-executor-device.hpp"
#include <memory>
#include <modules-common/factory.hpp>
#include <mutex>

class SynkExecutorDeviceModule : public ExecutorDeviceModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "synk-executor-device", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<ExecutorDeviceInterface>
    make_executor_device(CPUContext &ctx, ModuleSettings &stt) const override {
        static Factory<std::mutex> mut_f;
        std::mutex &m = mut_f.make();
        return std::make_unique<SynkExecutorDevice>(
            ctx.instructions, ctx.req_manager, m
        );
    }
};

SynkExecutorDeviceModule synk_executor_device;

ModuleToken token(synk_executor_device);
