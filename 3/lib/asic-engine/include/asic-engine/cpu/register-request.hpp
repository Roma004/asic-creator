#pragma once
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <asic-engine/cpu/register-set.hpp>
#include <memory>

/**
 * @brief Запрос на владение регистрами
 *
 * Запрос в себе хранит набор указателей на ссылки регистров.
 * Задача этого класса - предоставить в необходимом для менеджера запросов
 * формате информацию о том, доступ к камик регистрам необходим.
 *
 * Когда владение всеми небходимыми регистрами будет получено, метод
 * is_ready() вернёт true. Это означает, что можно приступать к исполнению
 * опреации
 **/
class RegisterRequest {
    using register_t = RegisterBlock::RegisterReference;

  public:
    RegisterRequest(RegisterSet &regs);
    RegisterRequest(const RegisterRequest &) = delete;
    RegisterRequest(RegisterRequest &&) = default;

    /** @brief владеет ли запрос всеми назначенными регистрами */
    bool is_ready() const noexcept;

    /**
     * @brief Получить набор указателей на ссылки необходимых регистров
     * @return набор указателей на регистры
     */
    std::span<std::shared_ptr<register_t>> registers() noexcept;

    /** @brief проверить, запрашиавется ли данный регистр этим запросом */
    bool is_register_requested(unsigned int reg_id) const noexcept;

  protected:
    RegisterSet &regs;
};
