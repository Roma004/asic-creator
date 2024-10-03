#include "cpu/engine/data-master-port.hpp"
#include "data-master/abstracts/abstract-packet.hpp"
#include "data-master/abstracts/endpoints.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_section.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <memory>
#include <queue>

class TestRootPort: public RootInterface {
    std::queue<std::shared_ptr<AbstractPacket>> q;

    void send_request_pkt(std::shared_ptr<AbstractPacket> pkt) {
        if (pkt->get_type() == READ) {
            REQUIRE(pkt->get_addr() == q.front()->get_addr());
        } else {
            q.push(std::move(pkt));
        }
    }

    bool recv_response_pkt(std::shared_ptr<AbstractPacket> &pkt) {
        if (q.empty()) return false;
        pkt = q.front();
        q.pop();
        return true;
    }
};

TEST_CASE("Data Master CPU Port functionality", "[cpu][data_master_port]") {
    using namespace Catch::Matchers;
    TestRootPort enp;
    DataMasterPort port(enp);

    port.write8(1, 0xff);
    port.write16(2, 0xffff);
    port.write32(3, 0xffffffff);
    port.write64(4, 0xffffffffffffffff);
    port.write16(123, 0xffff);
    port.write16(123, 0xaabb);

    REQUIRE(port.read8(1) == 0xff);
    REQUIRE(port.read16(2) == 0xffff);
    REQUIRE(port.read32(3) == 0xffffffff);
    REQUIRE(port.read64(4) == 0xffffffffffffffff);
    REQUIRE_THROWS_WITH(port.read64(123) == 0xffff, ContainsSubstring("Invalid read"));
    REQUIRE_FALSE(port.read16(123) == 0xffff);
}
