#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "riscv-lui-auipc-functor-x32/riscv-lui-auipc-functor-x32.hpp"

class RiscVLuiAuiPCFunctorX32Module : public InstructionFunctorModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "riscv-lui-auipc-functor-x32", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const override {
        return std::make_unique<UFunctor>(ctx.ctrl->get_pc());
    }
};

RiscVLuiAuiPCFunctorX32Module riscv_lui_auipc_functor_x32;

ModuleToken token(riscv_lui_auipc_functor_x32);
