#include "asic-engine/cpu/abstracts/executor-device.hpp"
#include "asic-engine/cpu/modules/executor-device-module.hpp"
#include "asic-engine/cpu/registries/cpu-context.hpp"
#include "asynk-executor-device/asynk-executor-device.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include <memory>
#include <modules-common/factory.hpp>

class AsynkExecutorDeviceModule : public ExecutorDeviceModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "asynk-executor-device", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<ExecutorDeviceInterface>
    make_executor_device(CPUContext &ctx, ModuleSettings &stt) const override {
        static Factory<std::mutex> mut_f;
        std::mutex &m = mut_f.make();
        return std::make_unique<AsynkExecutorDevice>(
            ctx.instructions, ctx.req_manager, m
        );
    }
};

AsynkExecutorDeviceModule asynk_executor_device;

ModuleToken token(asynk_executor_device);
