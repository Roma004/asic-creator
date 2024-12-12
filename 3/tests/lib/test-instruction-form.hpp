#pragma once
#include "asic-engine/cpu/abstracts/registers.hpp"
#include "asic-engine/cpu/base-instruction.hpp"
#include "test-register-block.hpp"
#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <ranges>

class InstructionFormMock : public InstructionForm {
  public:
    constexpr const static std::string form = "ASD";
    std::vector<int> rsids;
    int rd_id;
    TestRegisterBlock &reg_blk;

    InstructionFormMock(
        int rd_id,
        const std::initializer_list<int> &rs_ids,
        TestRegisterBlock &reg_blk
    ) : rsids(rs_ids), rd_id(rd_id), reg_blk(reg_blk) {}

    const std::string &get_form() const noexcept override { return form; }

    RegisterSet get_registers(BaseInstruction i) const override {
        auto v = std::ranges::transform_view(rsids, [this](int idx) {
            return idx != -1 ? reg_blk.make_reference(idx) : nullptr;
        });
        auto rd = rd_id != -1 ? reg_blk.make_reference(rd_id) : nullptr;
        if (v.size() == 0) return RegisterSet(rd);
        if (rd_id != -1) {
            if (i.get_instruction() == 0)
                return RegisterSet(rd, v.begin(), v.end());
            else return RegisterSet(rd, v.end(), v.begin());
        } else {
            if (i.get_instruction() == 0)
                return RegisterSet(v.begin(), v.end());
            else return RegisterSet(v.end(), v.begin());
        }
    }

    uint32_t get_immediate(BaseInstruction) const override { return 0; };

    uint32_t get_minor(BaseInstruction) const override { return 0; }

    uint32_t get_sub_minor(BaseInstruction) const override { return 0; }
};

class InstructionFormTest : public InstructionForm {
  public:
    constexpr const static std::string form = "TEST";
    TestRegisterBlock &blk_rd;
    TestRegisterBlock &blk_rs;

    struct descriptor {
        uint32_t opcode: 7;
        uint32_t rd    : 5;
        uint32_t rs1   : 5;
        uint32_t rs2   : 5;
        uint32_t imm   : 7;
        uint32_t minor : 3;

        void set(uint32_t i) { *this = *(descriptor *)&i; }
        operator uint32_t() const { return *(uint32_t *)this; }
    };

    InstructionFormTest(TestRegisterBlock &blk_rd, TestRegisterBlock &blk_rs) :
        blk_rd(blk_rd), blk_rs(blk_rs) {}

    const std::string &get_form() const noexcept override { return form; }

    RegisterSet get_registers(BaseInstruction i) const override {
        descriptor instr;
        instr.set(i.get_instruction());
        auto rd = blk_rd.make_reference(instr.rd);
        auto rs1 = blk_rs.make_reference(instr.rs1);
        auto rs2 = blk_rs.make_reference(instr.rs2);
        return RegisterSet(rd, {rs1, rs2});
    }

    uint32_t get_immediate(BaseInstruction i) const override {
        descriptor d;
        d.set(i.get_instruction());
        return d.imm;
    };

    uint32_t get_minor(BaseInstruction i) const override {
        descriptor d;
        d.set(i.get_instruction());
        return d.minor;
    };

    uint32_t get_sub_minor(BaseInstruction) const override { return 0; }

    BaseInstruction format(
        char opcode, char rd, char rs1, char rs2, char imm, char minor
    ) const noexcept {
        descriptor desc = {
            (uint32_t)opcode,
            (uint32_t)rd,
            (uint32_t)rs1,
            (uint32_t)rs2,
            (uint32_t)imm,
            (uint32_t)minor
        };
        return BaseInstruction((uint32_t)desc);
    }
};

