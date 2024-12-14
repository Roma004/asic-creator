#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/device-gate.hpp"
#include "modules-common/factory.hpp"
#include <asic-engine/data-master/modules/device-gate-module.hpp>
#include <memory>

class DiviceGateAnyPktModule : public DeviceGateModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "device-gate-anypkt", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<EndpointInterface> make_device_gate(ModuleSettings &stt
    ) const override {
        static Factory<PacketQueue> qf;
        auto &in_q = qf.make();
        auto &out_q = qf.make();
        return std::make_unique<DeviceGate>(in_q, out_q);
    }
};

DiviceGateAnyPktModule device_gate_anypkt;

ModuleToken token(device_gate_anypkt);
