#include "asic-engine/cpu/abstracts/data-master-port.hpp"
#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/data-master-port.hpp"
#include <asic-engine/cpu/modules/data-master-port-module.hpp>
#include <memory>

class PortRW1248iIPktModule : public DataMasterPortModule {

    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "port-rw1248i-ipkt", "v1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<DataMasterPortInterface>
    make_port(RootInterface &gate, ModuleSettings &stt) const override {
        return std::make_unique<DataMasterPort>(gate);
    }
};

PortRW1248iIPktModule port_rw1248i_ipkt_mod;

ModuleToken token(port_rw1248i_ipkt_mod);

