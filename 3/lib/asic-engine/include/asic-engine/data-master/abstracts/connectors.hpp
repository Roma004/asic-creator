#pragma once
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <functional>
#include <memory>

/**
 * @brief Принимающий интерфейс шины
 *
 * Интерфейс принимает пакеты извне, т.е. в него можно послать пакет.
 *
 * Реализует метод `send` для отправки пакета в этот интерфейс.
 * */
class Receiver {
  public:
    /** @brief послать пакет в принимающий интерфейс */
    virtual void send(std::shared_ptr<PacketInterface> pkt) = 0;
};

/**
 * @brief Отдающий интерфейс шины
 *
 * Интерфейс отдаёт пакеты получателям извне, т.е. из него можно получить пакет.
 *
 * Реализует метод `recv` для попытки получить пакет.
 * */
class Sender {
  public:
    using cond_t = std::function<bool(std::shared_ptr<PacketInterface>)>;

    /**
     * @brief Получить пакет с заданными параметрами, если он имеется
     * @param[pkt]
     *   Ссылка на мето в памяти, куда надо положить принятый пакет, если он
     *   будет принят
     * @param[recv_if] условие извлечения пакета из интерфейса.
     * */
    virtual bool
    recv(std::shared_ptr<PacketInterface> &pkt, cond_t recv_if) = 0;
};
