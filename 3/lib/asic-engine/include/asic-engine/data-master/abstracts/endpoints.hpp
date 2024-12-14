#pragma once
#include "asic-engine/data-master/abstracts/packet.hpp"
#include "asic-engine/data-master/abstracts/transfer-interface.hpp"
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <memory>

/**
 * @brief Интерфейс ведомого устройства шины
 *
 * Описывает интерфейс шины, подключаемый к ведущему устройству, способный
 * принимать запросы от ведущего, пересылать своему ведомому, и наоборот.
 * */
class SlaveInterface {
  public:
    virtual ~SlaveInterface() = default;
    /**
     * @brief Получить объект получателя пакетов со стороны ведущего устройства
     * */
    virtual Receiver &get_receiver() noexcept = 0;

    /**
     * @brief Получить объект отправителя пакетов в сторону ведущего устройства
     * */
    virtual Sender &get_sender() noexcept = 0;
};

/**
 * @brief Интерфейс оконечного устройства шины
 *
 * Реализует входной интерфейс оконечного устройства шины, т.е. получателя
 * пакета.
 *
 * Определяет методы извлечения данных, пришедших с шины и отправки ответа
 * отправителю помимо унаследованных от SlaveInterface методов
 * приёма/получения пакетов шиной из интерфейса.
 **/
class EndpointInterface : public SlaveInterface {
  public:
    virtual ~EndpointInterface() = default;
    /** @brief Послать пакет ответа отправителю */
    virtual void send_response_pkt(std::shared_ptr<PacketInterface> pkt) = 0;

    /**
     * @brief Извлечь пакет запроса пришетший извне, если он есть
     * @return
     *   true -- если извлечение пакета успешно
     *   false -- если не успешно
     * */
    virtual std::optional<std::shared_ptr<PacketInterface>>
    recv_request_pkt() = 0;
};

/**
 * @brief Интерфейс корневого устройства шины
 *
 * Описывает входной интерфейс корневого устройства шины, т.е. отправителя
 * пакета.
 **/
class RootInterface {
  public:
    virtual ~RootInterface() = default;
    /** @brief Послать пакет запроса получателю */
    virtual void send_request_pkt(std::shared_ptr<PacketInterface> pkt) = 0;

    /**
     * @brief Извлечь пакет ответа пришетший извне, если он есть
     * @return
     *   true -- если извлечение пакета успешно
     *   false -- если не успешно
     * */
    virtual std::optional<std::shared_ptr<PacketInterface>>
    recv_response_pkt() = 0;
};

/**
 * @brief Интерфейс ветвителя маршрута шины
 *
 * Описывает интерфейс специального устройства, задача которого распределять
 * пакеты пришедшие от ведущего устрйоства по N ведомым устройствам в
 * соответствиии с заданным ему набором диапазонов адрессов.
 **/
class SwitchInterface : public SlaveInterface, public TransferInterface {
  public:
    virtual ~SwitchInterface() = default;
    /**
     * @brief Подключить список ведомых устройств
     * @param[l]
     *   список инициализации, содержащи информацию о диапазне адрессов, при
     *   попадании в который пакет должден быть перенаправлен на конкретное
     *   устройство
     * @throws TODO
     * */
    virtual void connect_slaves(
        const std::initializer_list<std::pair<AddrRange, SlaveInterface &>> &l
    ) = 0;

    /**
     * @brief Подключить ведомое устрйоство к конкретному диапазону адрессов
     * @param[rng] диапазон адрессов, в который должно смотреть устройство
     * @param[slv]
     *   Ведомое устрйоство, которое должно быть поключено к заданному
     *   диапазону адрессов
     * @throws TODO
     * */
    virtual void connect_slave(AddrRange rng, SlaveInterface &slv) = 0;
};

/**
 * @brief Интерфейс моста шины
 *
 * Описывает интерфейс устройства, задача которого принимать пакеты пришедшие от
 * ведущего устрйоства и передавать их ведомому.
 *
 * Мост может быть применим в случае, когда есть потребность в преобразовании
 * пакета запроса перед передачей ведомому.
 *
 * К примеру, когда ведущий отдаёт пакеты строго определённого типа, но есть
 * ведомые, подключенные через ветвитель, принимающие пакеты разных типов.
 *
 * Другим вариантом может быть потребность в реализации регистратора пакетов,
 * идущих по шине. Так, подключив его в конкретную ветку можно собирать
 * информацию, о данных идущих по этой шине, не нарушая при этом целостности
 * и не модифицируя код других устройств.
 **/
class BridgeInterface : public SlaveInterface, public TransferInterface {
  public:
    virtual ~BridgeInterface() = default;
};

/**
 * @brief Интерфейс обработчика неверных запросов
 *
 * Задача этого устройства, будучи подключённым к переключателю в качестве
 * обработчика пакетов, не подходящих ни под один адрессный диапазон, обработать
 * принятый пакет, и если опреация требует ответа - вернуть пакет-заглушку
 *
 * Обработчику не запрещается произвести с пакетом осмысленное действие, если
 * это предполагает логика, но в общем случае, максимум, что он может сделать -
 * записать лог о приедшем невалидном пакете.
 **/
class DefaultDeviceInterface : public SlaveInterface, public TransferInterface {
  public:
    virtual ~DefaultDeviceInterface() = default;
};
