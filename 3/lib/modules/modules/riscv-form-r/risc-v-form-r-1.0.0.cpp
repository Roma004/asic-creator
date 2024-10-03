#include "asic-engine/cpu/modules/instruction-form-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "risc-v-form-r/risc-v-form-r.hpp"

class RiscVFormRModule : public InstructionFormModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {"riscv-form-r", "1.0.0"};
        return module_info;
    }

    std::unique_ptr<InstructionForm>
    make_form(CPUContext &ctx, ModuleSettings &stt) const override {
        auto &regs = dynamic_cast<RegistersX32 &>(
            ctx.register_blocks.get_component(stt.get_string("regs"))
        );
        return std::make_unique<RForm>(regs);
    }
};

RiscVFormRModule risc_v_form_r;

ModuleToken token(risc_v_form_r);
