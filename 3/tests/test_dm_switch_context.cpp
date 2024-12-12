#include <asic-engine/data-master/common.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <asic-engine/data-master/switch-context.hpp>

TEST_CASE("Working with SwitchContext", "[dm][switch_context]") {
    SwitchContext ctx;

    REQUIRE(ctx.get_top_id() == 0);
    REQUIRE_NOTHROW(ctx.push_switch(1));
    REQUIRE_NOTHROW(ctx.push_switch(2));
    REQUIRE_NOTHROW(ctx.push_switch(3));
    REQUIRE(ctx.get_top_id() == 3);
    REQUIRE_NOTHROW(ctx.push_switch(4));
    REQUIRE_NOTHROW(ctx.push_switch(5));
    REQUIRE_NOTHROW(ctx.push_switch(6));
    REQUIRE_NOTHROW(ctx.push_switch(7));
    REQUIRE_NOTHROW(ctx.push_switch(8));
    REQUIRE_THROWS(ctx.push_switch(9));
    REQUIRE(ctx.get_top_id() == 8);

    REQUIRE_THROWS(ctx.pop_switch(7));
    REQUIRE_NOTHROW(ctx.pop_switch(8));
    REQUIRE_NOTHROW(ctx.pop_switch(7));
    REQUIRE(ctx.get_top_id() == 6);
    REQUIRE_NOTHROW(ctx.pop_switch(6));
    REQUIRE_NOTHROW(ctx.pop_switch(5));
    REQUIRE_NOTHROW(ctx.pop_switch(4));
    REQUIRE_NOTHROW(ctx.pop_switch(3));
    REQUIRE_NOTHROW(ctx.pop_switch(2));
    REQUIRE(ctx.get_top_id() == 1);
    REQUIRE_NOTHROW(ctx.pop_switch(1));
    REQUIRE(ctx.get_top_id() == 0);
    REQUIRE_THROWS(ctx.pop_switch(0));
}
