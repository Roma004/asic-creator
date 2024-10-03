#include "asic-engine/data-master/packet-queue.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "switch-anypkt/switch-anypkt.hpp"
#include <asic-engine/data-master/modules/switch-module.hpp>
#include <memory>
#include <modules-common/factory.hpp>
#include <mutex>

class SwitchAnyPktModule : public SwitchModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"switch-anypkt", "v1.0.0"};
        return module_info;
    }

    std::unique_ptr<SwitchInterface> make_switch(
        uint8_t switch_id, SlaveInterface &default_slave, ModuleSettings &stt
    ) const override {
        static Factory<std::mutex> mut_f;
        static Factory<PacketQueue> q_f;
        std::mutex &in_m = mut_f.make();
        std::mutex &out_m = mut_f.make();
        PacketQueue &in_q = q_f.make(in_m);
        PacketQueue &out_q = q_f.make(out_m);
        return std::make_unique<Switch>(switch_id, in_q, out_q, default_slave);
    }
};

SwitchAnyPktModule switch_anypkt;

ModuleToken token(switch_anypkt);
