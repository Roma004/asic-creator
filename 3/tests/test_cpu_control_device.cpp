#include "cpu/abstracts/control-device.hpp"
#include "cpu/base-instruction.hpp"
#include "cpu/engine/control-device.hpp"
#include "cpu/engine/data-master-port.hpp"
#include "cpu/engine/executor-device.hpp"
#include "cpu/generic-instruction.hpp"
#include "cpu/register-request-manager.hpp"
#include "cpu/registries/instruction-registry.hpp"
#include "data-master/abstracts/endpoints.hpp"
#include "data-master/engine/default-device.hpp"
#include "data-master/engine/interconect-packet.hpp"
#include "data-master/engine/packet-data.hpp"
#include "lib/test-instruction-form.hpp"
#include "lib/test-register-block.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cpu/register-request.hpp>
#include <cpu/register-set.hpp>
#include <cstdint>
#include <memory>
#include <vector>

class InstructionMemory : public RootInterface {
  public:
    std::vector<uint32_t> instrs;
    int idx = 0;
    uint32_t addr;

    void add_instruction(uint32_t instr) { instrs.push_back(instr); }

    void send_request_pkt(std::shared_ptr<AbstractPacket> pkt) {
        addr = pkt->get_addr();
    }

    bool recv_response_pkt(std::shared_ptr<AbstractPacket> &pkt) {
        if (idx == instrs.size()) return false;
        auto data = std::make_shared<PacketDataX32>(instrs[idx++]);
        pkt = std::make_shared<InterconectPacket>(READ, addr, data);
        return true;
    }
};


TEST_CASE("Control Device functionality", "[cpu][control_device]") {
    using namespace Catch::Matchers;
    InstructionRegistry i_reg;
    InstructionMemory enp;
    DataMasterPort port(enp);
    RegisterRequestsManager req_manager;
    SynkExecutorDevice c_exec(i_reg, req_manager);
    ControlDevice c_dev(i_reg, c_exec, port);
}
