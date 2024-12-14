#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <asic-engine/cpu/register-set.hpp>
#include <asic-engine/exceptions.hpp>
#include <format>

GenericInstruction::GenericInstruction(
    const InstructionForm &form, BaseInstruction instruction
) :
    form(form), instr(instruction),
    regs(std::move(form.get_registers(instruction))), req(regs) {}

GenericInstruction::GenericInstruction(GenericInstruction &&i) noexcept :
    form(i.form), instr(i.instr), regs(std::move(i.regs)), req(regs) {}

const InstructionForm &GenericInstruction::get_form() const noexcept {
    return form;
}

BaseInstruction GenericInstruction::get_instruction() const noexcept {
    return instr;
}

GenericInstruction::register_t &GenericInstruction::get_rd() {
    if (!regs.has_rd())
        throw GenericInstructionExc(std::format(
            "Destination register is not supported for {}-instruction",
            form.get_form()
        ));
    return *regs.get_rd();
}

GenericInstruction::register_t &GenericInstruction::get_rsn(size_t n) {
    if (!regs.has_rsn(n))
        throw GenericInstructionExc(std::format(
            "Operand register with regno={} is not supported for "
            "{}-instruction",
            n,
            form.get_form()
        ));
    return *regs.get_rsn(n);
}

uint32_t GenericInstruction::get_immediate() const noexcept {
    return form.get_immediate(instr);
}

uint32_t GenericInstruction::get_opcode() const noexcept {
    return form.get_opcode(instr);
}

uint32_t GenericInstruction::get_minor() const noexcept {
    return form.get_minor(instr);
}

uint32_t GenericInstruction::get_sub_minor() const noexcept {
    return form.get_sub_minor(instr);
}

RegisterRequest &GenericInstruction::get_request() noexcept { return req; }
