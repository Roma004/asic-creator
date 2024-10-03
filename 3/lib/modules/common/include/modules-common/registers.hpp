

#pragma once
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class RegistersExc : public std::logic_error {
  public:
    explicit RegistersExc(const std::string &msg) : std::logic_error(msg) {}
};

//////////////////////////////// Registers /////////////////////////////////////

/** @brief Шаблонный блок регистров */
template <class T> class Registers : public RegisterBlock {
  public:
    class RegisterReference;

    explicit Registers(size_t n, regid_t zero_reg_id);
    Registers(const Registers &) = default;
    Registers(Registers &&) = default;

    Registers &operator=(const Registers &) = default;
    Registers &operator=(Registers &&) = default;

    std::shared_ptr<RegisterBlock::RegisterReference> make_reference(regid_t id
    ) override;

    bool is_register_locked(regid_t id) const override;

    constexpr size_t size() const noexcept override { return regs.size(); }

  protected:
    virtual void borrow_register(RegisterBlock::RegisterReference &ref
    ) noexcept override;

    virtual void aquire_register(RegisterBlock::RegisterReference &ref
    ) noexcept override;

  private:
    struct Register {
        T value;
        regid_t id;
        bool locked;
    };

    std::vector<Register> regs;
    regid_t zero_reg_id;
};

using RegistersX64 = Registers<uint64_t>;
using RegistersX32 = Registers<uint32_t>;
using RegistersX16 = Registers<uint16_t>;
using RegistersX8 = Registers<uint8_t>;

//////////////////////////// RegisterReference /////////////////////////////////

template <class T>
class Registers<T>::RegisterReference :
    public RegisterBlock::RegisterReference {
  public:
    RegisterReference(Registers &reg_blk, Register &reg, bool is_zero);
    RegisterReference(RegisterReference &&o) = default;
    RegisterReference(const RegisterReference &) = delete;
    ~RegisterReference() override = default;

    RegisterReference &operator=(const RegisterReference &) = default;
    RegisterReference &operator=(RegisterReference &&) = default;

    bool is_aquired() const noexcept override;
    bool is_borrowed() const noexcept override;
    regid_t get_register_id() const override;
    RegisterBlock &get_register_block() const override;

    const T &get_value() const;
    void set_value(const T &val);
    void set_value(T &&val);

    bool operator==(const RegisterBlock::RegisterReference &o) const override;

  private:
    friend class Registers;

    Register &reg;
    Registers &reg_blk;
    bool borrowed = false;
    bool aquired = false;
    bool is_zero;
};

//////////////////////////// Registers Methods /////////////////////////////////

template <class T>
void Registers<T>::borrow_register(RegisterBlock::RegisterReference &ref
) noexcept {
    RegisterReference &r = dynamic_cast<RegisterReference &>(ref);
    r.borrowed = true;
    if (ref.get_register_id() == zero_reg_id) return;
    regs[r.get_register_id()].locked = true;
}

template <class T>
void Registers<T>::aquire_register(RegisterBlock::RegisterReference &ref
) noexcept {
    RegisterReference &r = dynamic_cast<RegisterReference &>(ref);
    if (r.is_aquired()) return;
    r.borrowed = false;
    r.aquired = true;
    if (ref.get_register_id() == zero_reg_id) return;
    regs[r.get_register_id()].locked = false;
}

template <class T>
std::shared_ptr<RegisterBlock::RegisterReference>
Registers<T>::make_reference(regid_t id) {
    return std::make_shared<RegisterReference>(
        *this, regs.at(id), id == zero_reg_id
    );
}

template <class T> bool Registers<T>::is_register_locked(regid_t id) const {
    return regs.at(id).locked;
}

template <class T>
Registers<T>::Registers(size_t n, regid_t zero_reg_id) :
    regs(n), zero_reg_id(zero_reg_id) {}

//////////////////////// RegisterReference Methods /////////////////////////////

template <class T>
Registers<T>::RegisterReference::RegisterReference(
    Registers &reg_blk, Register &reg, bool is_zero
) : reg(reg), reg_blk(reg_blk), is_zero(is_zero) {}

template <class T>
bool Registers<T>::RegisterReference::is_aquired() const noexcept {
    return aquired;
}

template <class T>
bool Registers<T>::RegisterReference::is_borrowed() const noexcept {
    return borrowed;
}

template <class T>
regid_t Registers<T>::RegisterReference::get_register_id() const {
    return reg.id;
}

template <class T>
RegisterBlock &Registers<T>::RegisterReference::get_register_block() const {
    return reg_blk;
}

template <class T> const T &Registers<T>::RegisterReference::get_value() const {
    if (!is_borrowed())
        throw RegistersExc("Attempt to use unborrowed register");
    return reg.value;
}

template <class T>
void Registers<T>::RegisterReference::set_value(const T &val) {
    if (!is_borrowed())
        throw RegistersExc("Attempt to use unborrowed register");
    if (is_zero) return;
    reg.value = val;
}

template <class T> void Registers<T>::RegisterReference::set_value(T &&val) {
    if (!is_borrowed())
        throw RegistersExc("Attempt to use unborrowed register");
    if (is_zero) return;
    reg.value = val;
}

template <class T>
bool Registers<T>::RegisterReference::operator==(
    const RegisterBlock::RegisterReference &o
) const {
    return &reg_blk == &o.get_register_block() && reg.id == o.get_register_id();
}
