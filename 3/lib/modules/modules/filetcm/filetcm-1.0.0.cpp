#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include "asic-engine/peripheral/modules/peripheral-module.hpp"
#include "include/filetcm/filetcm.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/device-gate.hpp"
#include <memory>

class FileTCMModule : public PeripheralModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"filetcm", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<PeripheralInterface> make_peripheral(
        EndpointInterface &gate, const ModuleSettings &stt
    ) const override {
        const std::string &filename = stt.get_string("tcm_file");
        size_t max_size = stt.get_int("max_size");
        return std::make_unique<FileTcm>(
            filename.c_str(), max_size, dynamic_cast<DeviceGate &>(gate)
        );
    }
};

FileTCMModule filetcm;

ModuleToken token(filetcm);
