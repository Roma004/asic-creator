#include "asic-engine/cpu/abstracts/instruction-functor.hpp"
#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "include/riscv-branch-functor-x32/riscv-branch-functor-x32.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "mod-loader/module-settings.hpp"
#include <memory>

class RiscVBranchFunctorX32Module : public InstructionFunctorModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "riscv-branch-functor-x32", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const override {
        return std::make_unique<BFunctor>(ctx.ctrl->get_pc());
    }
};

RiscVBranchFunctorX32Module riscv_branch_functor_x32;

ModuleToken token(riscv_branch_functor_x32);
