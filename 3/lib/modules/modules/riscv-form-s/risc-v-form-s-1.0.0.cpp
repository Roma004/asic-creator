#include "asic-engine/cpu/modules/instruction-form-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "risc-v-form-s/risc-v-form-s.hpp"

class RiscVFormSModule : public InstructionFormModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"riscv-form-s", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<InstructionForm>
    make_form(CPUContext &ctx, ModuleSettings &stt) const override {
        auto &regs = dynamic_cast<RegistersX32 &>(
            ctx.register_blocks.get_component(stt.get_string("regs"))
        );
        return std::make_unique<SForm>(regs);
    }
};

RiscVFormSModule risc_v_form_s;

ModuleToken token(risc_v_form_s);
