#pragma once
#include <asic-engine/data-master/abstracts/connectors.hpp>
#include <asic-engine/data-master/abstracts/endpoints.hpp>
#include <asic-engine/data-master/abstracts/packet.hpp>
#include <asic-engine/data-master/common.hpp>
#include <memory>

// TODO нельзя идентифицировать запросы только по адресу результата
// необходимо дополнять пакет специальным полем, которое будет определять
// отправителя пакета
// Но это отложенная задача
class CPUGate : public RootInterface {
  public:
    CPUGate(Sender &slave_sender, Receiver &slave_receiver);

    void send_request_pkt(std::shared_ptr<PacketInterface> pkt
    ) noexcept override;

    bool recv_response_pkt(std::shared_ptr<PacketInterface> &pkt
    ) noexcept override;

  private:
    addr_t address;
    Receiver &slave_receiver;
    Sender &slave_sender;
};

