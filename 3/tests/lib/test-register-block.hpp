#pragma once
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class TestRegisterBlockExc : public std::logic_error {
  public:
    explicit TestRegisterBlockExc(const std::string &msg) :
        std::logic_error(msg) {}
};

class TestRegisterBlock : public RegisterBlock {
  public:
    struct Register {
        uint32_t value;
        bool locked;
    };

    class RegRef : public RegisterBlock::RegisterReference {
      public:
        Register &reg;
        TestRegisterBlock &reg_blk;
        regid_t id;
        bool borrowed = false;

        RegRef(TestRegisterBlock &reg_blk, Register &reg, regid_t id) :
            reg(reg), reg_blk(reg_blk), id(id) {
#ifdef DEBUG
            std::cerr << "RegRef(this=" << this << ", id=" << id << ")"
                      << std::endl;
#endif
        }
        RegRef(RegRef &&o) :
            reg_blk(o.reg_blk), reg(o.reg), id(o.id), borrowed(o.borrowed) {
            reg_blk.references.erase(std::ranges::find(reg_blk.references, &o));
            reg_blk.references.push_back(this);
        }
        RegRef(const RegRef &) = delete;
        ~RegRef() override {
#ifdef DEBUG
            std::cerr << "~RegRef() with this=" << this << " (reg_id=" << id
                      << ") {";
            for (auto i : reg_blk.references) std::cerr << " " << i;
            std::cerr << " }" << std::endl;
#endif
            reg_blk.references.erase(std::ranges::find(reg_blk.references, this)
            );
        }

        bool is_aquired() const noexcept override { return !borrowed; }
        bool is_borrowed() const noexcept override { return borrowed; }
        regid_t get_register_id() const override { return id; }
        RegisterBlock &get_register_block() const override { return reg_blk; }

        bool operator==(const RegisterBlock::RegisterReference &o
        ) const override {
            return &reg_blk == &o.get_register_block()
                && id == o.get_register_id();
        }
    };

    std::vector<RegRef *> references;

    std::vector<Register> regs;

    explicit TestRegisterBlock(size_t n) : regs(n), references(0) {}
    TestRegisterBlock(const TestRegisterBlock &) = default;
    TestRegisterBlock(TestRegisterBlock &&) = default;

    std::shared_ptr<RegisterReference> make_reference(regid_t id) override {
        auto r = std::make_shared<RegRef>(*this, regs.at(id), id);
        references.push_back(r.get());
        return dynamic_pointer_cast<RegisterReference>(r);
    }

    bool is_register_locked(regid_t id) const override {
        return regs.at(id).locked;
    }

    void print_references() const {
#ifdef DEBUG
        std::cerr << "{ ";
        for (auto i : references) std::cerr << i << " ";
        std::cerr << "}" << std::endl;
#endif
    }

    void print_regs() const noexcept {
        for (int i = 0; i < regs.size(); ++i)
            std::cout << std::format("reg{}:\t{}\n", i, regs[i].value);
        std::cout << std::endl;
    }

    constexpr size_t size() const noexcept override { return regs.size(); }

    void public_borrow(RegisterBlock::RegisterReference &ref) {
        borrow_register(ref);
    }

    void public_aquire(RegisterBlock::RegisterReference &ref) {
        aquire_register(ref);
    }

  protected:
    virtual void borrow_register(RegisterBlock::RegisterReference &ref
    ) noexcept override {
        RegRef &r = dynamic_cast<RegRef &>(ref);
        r.borrowed = true;
        regs[r.id].locked = true;
    }

    virtual void aquire_register(RegisterBlock::RegisterReference &ref
    ) noexcept override {
        RegRef &r = dynamic_cast<RegRef &>(ref);
        if (r.is_aquired()) return;
        r.borrowed = false;
        regs[r.id].locked = false;
    }
};
