#pragma once
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <asic-engine/cpu/register-request.hpp>
#include <functional>
#include <list>
#include <mutex>

/**
 * @brief Класс - аггрегатор запросов на владение регистрами
 *
 * Для того, чтобы получить доступ на чтение/запись к регистру надо запросить у
 * блока регистров владение этими регистрами.
 * Разрешением конфликтов владения между исполняющими устройствами занимается
 * этот класс.
 *
 * Каждый приходящий запрос встаёт в очередь на получение ресурсов.
 * Для получения статуса готовности запроса следует обращаться к методу
 * переданного объекта запроса is_ready().
 *
 * Как только исполняющее устройство завершило своё исполнение, оно обязано
 * вернуть влдение всеми запрошенными регистрами при помощи метода
 * free_registers() этого класса.
 * */
class RegisterRequestsManager {
    using register_t = RegisterBlock::RegisterReference;

  public:
    RegisterRequestsManager(std::mutex &m);
    /**
     * @brief добавить запрос на владение регистрами
     * @param[req] объект запроса на владение регистрами
     **/
    void add_request(RegisterRequest &req) noexcept;

    /**
     * @brief освободить ресурсы от выполненного запроса
     * @param[req]
     *   объект запроса на владение регистрами ресурсы которого надо освободить
     **/
    void free_registers(RegisterRequest &req) noexcept;

  private:
    using reqlist_t = std::list<std::reference_wrapper<RegisterRequest>>;

    bool extract_if_ready(reqlist_t::iterator &it) noexcept;

    void send_possession(RegisterRequest &from, RegisterRequest &to) noexcept;

    reqlist_t requests_list;
    std::mutex &m;
};
