#include "asic-engine/cpu/modules/instruction-functor-module.hpp"
#include "mod-loader/module-interface.hpp"
#include "mod-loader/module-manager.hpp"
#include "modules-common/data-master-port.hpp"
#include "riscv-load-functor-x32/riscv-load-functor-x32.hpp"

class RiscVLoadFunctorX32Module : public InstructionFunctorModule {
    const ModuleBasicInfo &get_info() const override {
        static const ModuleBasicInfo module_info = {
            "riscv-load-functor-x32", "1.0.0"
        };
        return module_info;
    }

    std::unique_ptr<InstructionFunctor>
    make_functor(CPUContext &ctx, ModuleSettings &stt) const override {
        return std::make_unique<LFunctor>(dynamic_cast<DataMasterPort &>(
            ctx.ports.get_component(stt.get_string("port"))
        ));
    }
};

RiscVLoadFunctorX32Module riscv_load_functor_x32;

ModuleToken token(riscv_load_functor_x32);
