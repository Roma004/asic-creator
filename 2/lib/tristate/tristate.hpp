#pragma once

#include <istream>
#include <ostream>

/*!
 * \addtogroup TristateLogic
 * @{
 *
 * # Introduction to tristate logic rules.
 *
 * tristate logic adds the third `undefined` (X) state to the standard True and
 * false. It means it is neither logical 1 nor 0, so that we can define the
 * following logical rulles:
 *
 * ## Logical NOT
 * x|res
 * -|----
 * 0| 1
 * 1| 0
 * X| X
 *
 * ## Logical OR
 *
 * x|y|res
 * -|-|----
 * 0|0| 0
 * 0|1| 1
 * 0|X| X
 * 1|0| 1
 * 1|1| 1
 * 1|X| 1
 * X|0| X
 * X|1| 1
 * X|X| X
 *
 * ## Logical AND
 * x|y|res
 * -|-|----
 * 0|0| 0
 * 0|1| 0
 * 0|X| 0
 * 1|0| 0
 * 1|1| 1
 * 1|X| X
 * X|0| 0
 * X|1| X
 * X|X| X
 *
 * @}
 */

/**
 * @brief
 *     calss represents Tri-state logical point able to have True, False and
 *     undefined state
 * */
class Tristate {
  public:
    /**
     * @brief enum describing available states of logical point
     * */
    enum STATE {
        F = 0,
        T = 1,
        X = -1,
    };

  private:
    STATE st;
    constexpr static STATE char2st(char c) noexcept {
        switch (c) {
        case 't':
        case 'T':
        case '1': return T;
        case 'f':
        case 'F':
        case '0': return F;
        default: return X;
        }
    }

    constexpr static STATE int2st(int c) noexcept {
        switch (c) {
        case 0: return F;
        case 1: return T;
        default: return X;
        }
    }
    constexpr static char st2char(STATE s) noexcept {
        switch (s) {
        case T: return '1';
        case F: return '0';
        default: return 'x';
        }
    }

  public:
    Tristate(Tristate::STATE st);

    /** @brief Defaults to X */
    Tristate();

    /**
     * @brief constructor from character signal
     *
     * Values interpreted as True:  't', 'T', '1'
     * Values interpreted as False: 'f', 'F', '0'
     * Others are interepreted as Undefined (X)
     * */
    Tristate(char c);

    /**
     * @brief constructor from integer
     *
     * Values interpreted as True:  1
     * Values interpreted as False: 0
     * Others are interepreted as Undefined (X)
     * */
    Tristate(int c);

    Tristate(const Tristate &) = default;
    Tristate(Tristate &&) = default;
    ~Tristate() = default;

    constexpr bool operator==(const Tristate &o) const noexcept {
        return st == o.st;
    }
    constexpr bool operator!=(const Tristate &o) const noexcept {
        return st != o.st;
    }
    constexpr friend bool operator==(STATE st, const Tristate &o) noexcept {
        return st == o.st;
    }
    constexpr friend bool operator!=(STATE st, const Tristate &o) noexcept {
        return st != o.st;
    }
    constexpr bool operator==(STATE o) const noexcept { return st == o; }
    constexpr bool operator!=(STATE o) const noexcept { return st != o; }

    Tristate &operator=(const Tristate &o) noexcept;
    Tristate &operator=(STATE state) noexcept;
    Tristate &operator=(int state) noexcept;
    Tristate &operator=(char state) noexcept;

    // Tristate-Tristate operators

    /** @brief Logical NOT operator, works as defined in \ref TristateLogic */
    Tristate operator!() const noexcept;

    /** @brief Logical OR operator, works as defined in \ref TristateLogic */
    Tristate operator||(const Tristate &o) const noexcept;

    /** @brief Logical AND operator, works as defined in \ref TristateLogic */
    Tristate operator&&(const Tristate &o) const noexcept;

    /** @brief Binary NOT operator, works as defined in \ref TristateLogic */
    Tristate operator~() const noexcept;

    /** @brief Binary OR operator, works as defined in \ref TristateLogic */
    Tristate operator|(const Tristate &o) const noexcept;

    /** @brief Binary AND operator, works as defined in \ref TristateLogic */
    Tristate operator&(const Tristate &o) const noexcept;

    // Tristate-STATE operators

    /** @brief Logical OR operator, works as defined in \ref TristateLogic */
    Tristate operator||(STATE o) const noexcept;

    /** @brief Logical AND operator, works as defined in \ref TristateLogic */
    Tristate operator&&(STATE o) const noexcept;

    /** @brief Binary OR operator, works as defined in \ref TristateLogic */
    Tristate operator|(STATE o) const noexcept;

    /** @brief Binary AND operator, works as defined in \ref TristateLogic */
    Tristate operator&(STATE o) const noexcept;

    // STATE-Tristate operators

    /** @brief Logical OR operator, works as defined in \ref TristateLogic */
    friend Tristate operator||(STATE st, const Tristate &o) noexcept {
        return o || st;
    }

    /** @brief Logical AND operator, works as defined in \ref TristateLogic */
    friend Tristate operator&&(STATE st, const Tristate &o) noexcept {
        return o && st;
    }

    /** @brief Binary OR operator, works as defined in \ref TristateLogic */
    friend Tristate operator|(STATE st, const Tristate &o) noexcept {
        return o | st;
    }

    /** @brief Binary AND operator, works as defined in \ref TristateLogic */
    friend Tristate operator&(STATE st, const Tristate &o) noexcept {
        return o & st;
    }

    // STATE-STATE operators

    /** @brief Logical OR operator, works as defined in \ref TristateLogic */
    friend Tristate operator||(STATE st, STATE o) noexcept {
        return Tristate(st) || o;
    }

    /** @brief Logical AND operator, works as defined in \ref TristateLogic */
    friend Tristate operator&&(STATE st, STATE o) noexcept {
        return Tristate(st) && o;
    }

    /** @brief Binary OR operator, works as defined in \ref TristateLogic */
    friend Tristate operator|(STATE st, STATE o) noexcept {
        return Tristate(st) | o;
    }

    /** @brief Binary AND operator, works as defined in \ref TristateLogic */
    friend Tristate operator&(STATE st, STATE o) noexcept {
        return Tristate(st) & o;
    }

    /** @brief Logical NOT operator, works as defined in \ref TristateLogic */
    friend Tristate operator!(STATE st) noexcept { return !Tristate(st); }

    /** @brief Binary NOT operator, works as defined in \ref TristateLogic */
    friend Tristate operator~(STATE st) noexcept { return ~Tristate(st); }

    /** @brief write character-representation of Tristate to std::ostream */
    friend std::ostream &operator<<(std::ostream &out, const Tristate &st) {
        out << (char)st;
        return out;
    }

    /** @brief get character-representation of Tristate from std::istream */
    friend std::istream &operator>>(std::istream &in, Tristate &st) {
        char sg;
        in >> sg;
        st = sg;
        return in;
    }

    /** @brief Convert to char
     *  @return char by next rule: T=>'1', F=>'0', X=>'x'
     * */
    constexpr operator char() const noexcept { return st2char(st); }

    /** @brief Convert to Tristate::STATE
     *  @return Corresponding member of Tristate::STATE enum
     * */
    constexpr operator STATE() const noexcept { return st; }
};
