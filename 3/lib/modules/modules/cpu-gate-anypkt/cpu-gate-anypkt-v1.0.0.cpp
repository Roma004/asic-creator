#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/data-master/modules/master-gate-module.hpp"
#include "cpu-gate-anypkt/cpu-gate-anypkt.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include <memory>

class CPUGateAnyPktModule : public MasterGateModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "cpu-gate-anypkt", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<RootInterface>
    make_master_gate(SlaveInterface &slv, ModuleSettings &stt) const override {
        return std::make_unique<CPUGate>(slv.get_sender(), slv.get_receiver());
    }
};

CPUGateAnyPktModule cpu_gate_anypkt;

ModuleToken token(cpu_gate_anypkt);
