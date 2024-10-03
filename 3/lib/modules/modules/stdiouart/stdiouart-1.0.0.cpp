#include "asic-engine/data-master/abstracts/endpoints.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"
#include "asic-engine/peripheral/modules/peripheral-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/device-gate.hpp"
#include "modules-common/stream-uart.hpp"
#include <iostream>
#include <memory>

class StdIOUARTModule : public PeripheralModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"stdiouart", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<PeripheralInterface>
    make_peripheral(EndpointInterface &gate, const ModuleSettings &)
        const override {
        return std::make_unique<StreamUART>(
            dynamic_cast<DeviceGate &>(gate), std::cin, std::cout
        );
    }
};

StdIOUARTModule stdiouart;

ModuleToken token(stdiouart);
