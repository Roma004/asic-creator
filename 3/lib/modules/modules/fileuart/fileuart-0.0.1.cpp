#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include "asic-engine/peripheral/modules/peripheral-module.hpp"
#include "include/fileuart/fileuart.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/device-gate.hpp"
#include <memory>

class FileUARTModule : public PeripheralModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"fileuart", "0.0.1"};
        return module_info;
    }

    std::unique_ptr<PeripheralInterface> make_peripheral(
        EndpointInterface &gate, const ModuleSettings &stt
    ) const override {
        const std::string &out_file = stt.get_string("out_filename");
        return std::make_unique<FileUART>(
            dynamic_cast<DeviceGate &>(gate), out_file
        );
    }
};

FileUARTModule fileuart;

ModuleToken token(fileuart);
