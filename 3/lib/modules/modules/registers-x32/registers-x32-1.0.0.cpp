#include "asic-engine/cpu/abstracts/registers.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/registers.hpp"
#include <asic-engine/cpu/modules/registers-module.hpp>
#include <memory>

class RegistersX32Module : public RegistersModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"registers-x32", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<RegisterBlock> make_registers(ModuleSettings &stt
    ) const override {
        return std::make_unique<Registers<uint32_t>>(
            stt.get_int("xlen"), stt.get_int("zero-id")
        );
    }
};

RegistersX32Module registers_x32;

ModuleToken token(registers_x32);
