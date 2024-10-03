#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "riscv-jump-functor-x32/riscv-jump-functor-x32.hpp"

class RiscVJumpFunctorX32Module : public InstructionFunctorModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "riscv-jump-functor-x32", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const override {
        return std::make_unique<JFunctor>(ctx.ctrl->get_pc());
    }
};

RiscVJumpFunctorX32Module riscv_jump_functor_x32;

ModuleToken token(riscv_jump_functor_x32);
