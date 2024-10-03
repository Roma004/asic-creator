#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

using regid_t = uint32_t;

/**
 * @brief Абстрактный класс блока регистров
 * */
class RegisterBlock {
  public:
    virtual ~RegisterBlock() {};
    /**
     * @brief Класс ссылки на конкретный регистр блока регистров
     *
     * Класс задаёт ссылку на один из регистров соответствующего блока регистров
     *
     * При конструировании обхекта класса через RegisterBlock::make_reference
     * производится ссылка, не владеющая ресурсами регистра, то есть, не имеющая
     * доступа на чтение/запись до значения регистра.
     *
     * Для получения доступа к состоянию регистра необходимо запросить этот
     * доступ. Подробнее см. RegisterRequest.
     **/
    class RegisterReference {
      public:
        /** @brief были ли ресурсы освобождены (валидна ли ещё ссылка) */
        virtual bool is_aquired() const noexcept = 0;

        /** @brief владеет ли регистр ресурсами в данный момент */
        virtual bool is_borrowed() const noexcept = 0;

        /** @brief получить порядковый номер в блоке регистров */
        virtual regid_t get_register_id() const = 0;

        /** @brief получить ссылку на соответствующий блок регистров */
        virtual RegisterBlock &get_register_block() const = 0;

        virtual ~RegisterReference() {};

        virtual bool operator==(const RegisterReference &) const = 0;
    };

    /** @brief получить ссылку на регистр (но не получать владение регистром) */
    virtual std::shared_ptr<RegisterReference> make_reference(regid_t reg_id
    ) = 0;

    /** @brief блокирует ли кто-то регистр */
    virtual bool is_register_locked(regid_t reg_id) const = 0;

    /** @brief количество регистров в блоке */
    virtual constexpr size_t size() const noexcept = 0;

  protected:
    /**
     * @brief сделать заданную ссылку владельцем регистра
     *
     * Метод не должен проверять, что владение ресурсом уже передано кому-то
     * так как такие запросы могут трактоваться только как запросы на владение
     * одним и тем же регистром в рамках одной и той же опреации.
     * Этот метод может вызываться только объектом RegisterRequestsManager и
     * только в случае, если на момент нахождения первого из повторяющихся
     * регистров в запросе этот регистр не являлся заблокированным.
     * Использование этого метода в других ситуациях не может происходить.
     *
     * Если предполагается возможность блокирования запросов извне, то для
     * этих целей следует следать отдельный метод, который уже будет
     * проверять, занят ли уже регистр.
     **/
    virtual void borrow_register(RegisterReference &ref) noexcept = 0;

    /** @brief вернуть владение данным регистром блоку регистров */
    virtual void aquire_register(RegisterReference &ref) noexcept = 0;

    friend class RegisterRequestsManager;
};
