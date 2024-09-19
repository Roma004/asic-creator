#pragma once

#include "tristate.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <tri-register-container.hpp>

/**
 * @brief class for processing registers (sequences) of \ref Tristate
 */
class TriRegister {
    TriRegisterContainer signals;

  public:
    /**
     * @brief
     *     Exception thrown on binary opeartion with registers of different
     *     length
     */
    class reg_length_error : public std::exception {
        friend class TriRegister;
        std::string msg;
        reg_length_error(
            const TriRegister &a, const TriRegister &b, const char *op
        ) noexcept;

      public:
        /**
         * @brief returns a message of what was happened
         * */
        virtual char const *what() const noexcept;
    };
    /** @brief regular forwards iterator */
    using iter = TriRegisterContainer::iterator;
    /** @brief constant forwards iterator */
    using citer = TriRegisterContainer::const_iterator;
    /** @brief regular backwards iterator */
    using riter = TriRegisterContainer::reverse_iterator;
    /** @brief constant backwards iterator */
    using rciter = TriRegisterContainer::const_reverse_iterator;

    using reference = TriRegisterContainer::reference;

    /** @brief make zero-length register by default */
    TriRegister() = default;

    /** @brief initialize register with nsigns Tristates of value st */
    TriRegister(size_t nsigns, Tristate st = Tristate::X) noexcept;

    /** @brief initialize register with content between from and to iterators */
    template <class Iter>
    TriRegister(Iter from, Iter to) noexcept : signals(from, to) {}
    /** @brief initialize register from c-array of length n */
    TriRegister(const auto *sgn, size_t n) noexcept;

    /** @brief initialize register from c-string */
    TriRegister(const char *str) noexcept;

    /** @brief initialize register with std::initializer_list */
    template <class T>
    TriRegister(const std::initializer_list<T> &l) noexcept :
        TriRegister(l.begin(), l.end()) {}

    TriRegister(const TriRegister &) = default;
    TriRegister(TriRegister &&) = default;
    ~TriRegister() = default;

    /** @brief output register as a string to std::ostream */
    friend std::ostream &operator<<(std::ostream &out, const TriRegister &reg) {
        std::ranges::for_each(reg, [&out](auto st) { out << (Tristate)st; });
        return out;
    }

    /** @brief input register as a string from std::istream */
    friend std::istream &operator>>(std::istream &in, TriRegister &reg) {
        std::ranges::for_each(reg, [&in](auto st) { in >> st; });
        return in;
    }

    /**
     * @brief get signal by index
     * @param idx -- index of signal to get
     * @return constant reference to the corresponding signal
     * @throw std::index_error on invalid index
     * */
    reference operator[](int idx);

    /** @brief tritwise inversion (binary NOT \ref TristateLogic) */
    TriRegister operator~() const noexcept;

    /**
     * @brief tritwise OR of arguments (\ref TristateLogic)
     * @return object of type TriRegister
     * @throw TriRegister::reg_length_error if arguments lengths not match
     */
    TriRegister operator|(const TriRegister &other) const;

    /**
     * @brief tritwise AND of arguments (\ref TristateLogic)
     * @return object of type TriRegister
     * @throw TriRegister::reg_length_error if arguments lengths not match
     */
    TriRegister operator&(const TriRegister &other) const;

    /**
     * @brief override with tritwise OR of arguments (\ref TristateLogic)
     * @throw TriRegister::reg_length_error if arguments lengths not match
     */
    TriRegister &operator|=(const TriRegister &other);

    /**
     * @brief override with tritwise AND of arguments (\ref TristateLogic)
     * @throw TriRegister::reg_length_error if arguments lengths not match
     */
    TriRegister &operator&=(const TriRegister &other);

    /** @brief copy-assignment operator */
    TriRegister &operator=(const TriRegister &other) noexcept;

    /** @brief move-assignment operator */
    TriRegister &operator=(TriRegister &&other) noexcept = default;

    /**
     * @brief compare two registers to be equal
     * @return
     *     true if all signals match
     *     false if some signals not match or length of registers differ
     * */
    bool operator==(const TriRegister &other) const noexcept;

    /**
     * @brief compare two registers not to be equal
     * @return
     *     false if all signals match
     *     true if some signals not match or length of registers differ
     * */
    bool operator!=(const TriRegister &other) const noexcept;

    /** @brief get number of signals in register */
    constexpr size_t size() const noexcept { return signals.size(); }

    /**
     * @brief check if register has X-values
     * @return true if has, false - overwise
     * */
    bool is_fully_defined() const noexcept;

    /** @brief add values of @p reg to the left of current register */
    void extend_left(const TriRegister &reg) noexcept;

    /** @brief add values of @p reg to the right of current register */
    void extend_right(const TriRegister &reg) noexcept;

    /** @brief regular iterator to the first (left) signal */
    iter begin() noexcept;
    /** @brief regular iterator to the end of signals sequence (left->right) */
    iter end() noexcept;

    /** @brief const iterator to the first (left) signal */
    citer begin() const noexcept;
    /** @brief const iterator to the end of signals sequence (left->right) */
    citer end() const noexcept;

    /** @brief const iterator to the first (left) signal */
    citer cbegin() const noexcept;
    /** @brief const iterator to the end of signals sequence (left->right) */
    citer cend() const noexcept;

    /** @brief regular iterator to the last (right) signal */
    riter rbegin() noexcept;
    /** @brief regular iterator to the end of signals sequence (right->left) */
    riter rend() noexcept;

    /** @brief const iterator to the last (right) signal */
    rciter rbegin() const noexcept;
    /** @brief const iterator to the end of signals sequence (right->left) */
    rciter rend() const noexcept;

    /** @brief const iterator to the last (right) signal */
    rciter crbegin() const noexcept;
    /** @brief const iterator to the end of signals sequence (right->left) */
    rciter crend() const noexcept;
};
