#pragma once

#include <asic-engine/cpu/abstracts/registers.hpp>
#include <asic-engine/exceptions.hpp>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

template <class It>
concept FwIter = requires() { requires std::forward_iterator<It>; };

/** @brief Класс для хранения набора регистров */
class RegisterSet {
    using register_t = RegisterBlock::RegisterReference;

  public:
    /**
     * @brief конструктор без регистра результата
     * @param[from] начало последовательности указателей на регистры операндов
     * @param[to] конец последовательности указателей на регистры операндов
     * @throw
     *   RegisterSetExc в случае если расстояние между \p from и \p to
     *   отрицательное
     * */
    template <FwIter It> RegisterSet(It from, It to);

    /**
     * @brief конструктор с регистрами результата и операндов
     * @param[rd] указатель на регистр результата
     * @param[from] начало последовательности указателей на регистры операндов
     * @param[to] конец последовательности указателей на регистры операндов
     * @throw
     *   RegisterSetExc в случае если расстояние между \p from и \p to
     *   отрицательное
     * */
    template <FwIter It>
    RegisterSet(std::shared_ptr<register_t> rd, It from, It to);

    RegisterSet(std::initializer_list<std::shared_ptr<register_t>> l);

    RegisterSet(
        std::shared_ptr<register_t> rd,
        std::initializer_list<std::shared_ptr<register_t>> l
    );

    /**
     * @brief конструктор с регистром результата
     * @param[rd] указатель на регистр результата
     * */
    explicit RegisterSet(std::shared_ptr<register_t> rd);

    RegisterSet(const RegisterSet &) = delete;
    RegisterSet(RegisterSet &&rs) = default;
    ~RegisterSet() = default;

    /**
     * @brief Получить регистр результата
     * @return указатель на регистр результата
     * @throw RegisterSetExc в случае если регистр результата не задан
     * */
    std::shared_ptr<register_t> get_rd();

    /**
     * @brief Получить регистр опреанда с номером \p n
     * @param[n] номер регистра операнда [1, rs_num)
     * @return указатель на регистр опреанда
     * @throw
     *   RegisterSetExc в случае попытки получить регистр с невалидным номером
     * */
    std::shared_ptr<register_t> get_rsn(size_t n);

    /**
     * @brief Получить набор указателей на регистры
     * @return
     *   набр указателей на регистры. (если регистр результата есть, он всегда
     *   идёт под номером 0)
     */
    std::span<std::shared_ptr<register_t>> registers() noexcept;

    /** @brief Получить поличество регистров опреандов */
    constexpr uint32_t get_rs_number() const noexcept;

    /** @brief Проверить наличие регистра результата */
    constexpr bool has_rd() const noexcept;

    /**
     * @brief Проверить наличие регистра опреанда с номером \p n
     * @param[n] номер регистра опреанда
     */
    constexpr bool has_rsn(size_t n) const noexcept;

  private:
    template <FwIter It> static size_t get_distance(It from, It to);

    std::vector<std::shared_ptr<register_t>> regs;
};

template <FwIter It> size_t RegisterSet::get_distance(It from, It to) {
    auto res = std::ranges::distance(from, to);
    if (res < 0)
        throw std::logic_error("Iterators with negative distance passed to "
                               "RegisterSet constructor");
    return res;
}

/** @brief конструктор без регистра результата */
template <FwIter It>
RegisterSet::RegisterSet(It from, It to) : RegisterSet(nullptr, from, to) {}

/** @brief конструктор с регистрами результата и операндов */
template <FwIter It>
RegisterSet::RegisterSet(std::shared_ptr<register_t> rd, It from, It to) :
    regs(get_distance(from, to) + 1) {
    regs[0] = rd;
    auto it = from;
    for (int i = 1; i < regs.size(); ++i, ++it) {
        if (*it == nullptr)
            throw RegisterSetExc(
                "nullptr is not allowed as a ponter to operand register"
            );
        regs[i] = std::move(*it);
    }
}

constexpr uint32_t RegisterSet::get_rs_number() const noexcept {
    return regs.size() - 1;
}

constexpr bool RegisterSet::has_rd() const noexcept { return bool(regs[0]); }

constexpr bool RegisterSet::has_rsn(size_t n) const noexcept {
    return n < regs.size() && n != 0;
}
