#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "default-device-anypkt-ipkt/default-device-anypkt-ipkt.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/factory.hpp"
#include <asic-engine/data-master/modules/default-device-module.hpp>
#include <asic-engine/data-master/packet-queue.hpp>
#include <memory>

class DefaultDeviceAnyPktIPktModule : public DefaultDeviceModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "default-device-anypkt-ipkt", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<DefaultDeviceInterface>
    make_default_device(ModuleSettings &stt) const override {
        static Factory<std::mutex> mut_f;
        static Factory<PacketQueue> q_f;
        std::mutex &in_m = mut_f.make();
        std::mutex &out_m = mut_f.make();
        PacketQueue &in_q = q_f.make(in_m);
        PacketQueue &out_q = q_f.make(out_m);
        return std::make_unique<DefaultDevice>(in_q, out_q);
    }
};

DefaultDeviceAnyPktIPktModule default_device_anypkt_ipkt;

ModuleToken token(default_device_anypkt_ipkt);
