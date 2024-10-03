#include "asic-engine/cpu/abstracts/instruction-form.hpp"
#include "asic-engine/cpu/modules/instruction-form-module.hpp"
#include "asic-engine/cpu/registries/cpu-context.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include "modules-common/registers.hpp"
#include "risc-v-form-b/risc-v-form-b.hpp"
#include <memory>

class RiscVFormBModule : public InstructionFormModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"riscv-form-b", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<InstructionForm>
    make_form(CPUContext &ctx, ModuleSettings &stt) const override {
        auto &regs = dynamic_cast<RegistersX32 &>(
            ctx.register_blocks.get_component(stt.get_string("regs"))
        );
        return std::make_unique<BForm>(regs);
    }
};

RiscVFormBModule risc_v_form_b;

ModuleToken token(risc_v_form_b);
