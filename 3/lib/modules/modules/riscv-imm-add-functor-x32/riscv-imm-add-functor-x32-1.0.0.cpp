#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "include/riscv-imm-add-functor-x32/riscv-imm-add-functor-x32.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"

class RiscVImmAddFunctorX32Module : public InstructionFunctorModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "riscv-imm-add-functor-x32", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const override {
        return std::make_unique<IFunctor>();
    }
};

RiscVImmAddFunctorX32Module riscv_imm_add_functor_x32;

ModuleToken token(riscv_imm_add_functor_x32);
