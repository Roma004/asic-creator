#pragma once
#include <asic-engine/data-master/common.hpp>
#include <asic-engine/data-master/switch-context.hpp>
#include <string>

/** @brief Бaзовый класс данных, содержащихся в пакете */
class PacketDataInterface {
  public:
    virtual ~PacketDataInterface() = default;
    /**
     * @brief Получить строковый идентификатор типа пакета
     *
     * Строковый идентификатор нужен для определения типа пришедших данных
     * Это может быть полезно, если оконечное устрйоство может принимать
     * пакеты разных типов (64, 32, 16, 8 бит разрядности) или, к примеру,
     * для проверки на стороне инициатора зпроса, что полученные с шины
     * данные не имеют тип "OOPS", сигнализирующий о неверно заданном адресе
     * */
    virtual constexpr const std::string &type_id() const noexcept = 0;

    /** @brief получить кол-во байт, требуемое для данных */
    virtual constexpr size_t sizeof_data() const noexcept = 0;

    virtual bool operator==(const PacketDataInterface &) const noexcept = 0;
};

/**
 * @brief Класс -- единица передачи данных по шине
 *
 * Пакет хранит в себе информацию:
 * - адрес, на который следует передать запрос
 * - данные, которые надо туда передать
 * - тип запроса -- чтение или запись
 */
class PacketInterface {
  public:
    /** @brief Получить тип опреации */
    virtual OPType get_type() const noexcept = 0;

    /** @brief Получить адрес получателя */
    virtual addr_t get_addr() const noexcept = 0;
    virtual void set_addr(addr_t addr) noexcept = 0;

    virtual const PacketDataInterface &get_data() const noexcept = 0;

    virtual ~PacketInterface() = default;

    /**
     * @brief добавить идентификатор транспортного узла
     *
     * При транспортироваке пакета, узлы, транспортирующие его, оставляют свои
     * идентификаторы. Таким образом есть возможность вернуть пакет ответа
     * отправителю той же траекторией, что он дошел до получателя.
     *
     * @returns Ссылка на внутренний объект контекста передачи данных.
     * */
    virtual SwitchContext &get_switch_context() = 0;
    virtual const SwitchContext &get_switch_context() const = 0;

    virtual bool operator==(const PacketInterface &o) const noexcept = 0;
};
