#include "tristate.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <ranges>
#include <tri-register-container.hpp>

#define use_trilogic          \
    using Tristate::STATE::T; \
    using Tristate::STATE::F; \
    using Tristate::STATE::X; \
    using st = Tristate::STATE;

#define REQ REQUIRE

TEST_CASE("TriRegisterContainer static assertions", "trc_static_assert") {
    using TRC = TriRegisterContainer;
    using iter = TriRegisterContainer::iterator;
    using sent = TriRegisterContainer::sentinel;

    TriRegisterContainer a, b;
    auto func1 = [](auto a, auto b) { return a | b; };

    static_assert(std::semiregular<Tristate>);

    static_assert(std::assignable_from<iter &, iter>);
    static_assert(std::movable<iter>);
    static_assert(std::__detail::__weakly_eq_cmp_with<sent, iter>);
    static_assert(std::weakly_incrementable<iter>);
    static_assert(std::input_or_output_iterator<iter>);

    static_assert(std::assignable_from<sent &, sent>);
    static_assert(std::movable<sent>);
    static_assert(std::copyable<sent>);
    static_assert(std::semiregular<sent>);

    static_assert(std::input_iterator<iter>);
    static_assert(std::sentinel_for<sent, iter>);

    static_assert(std::constructible_from<iter>);
    static_assert(std::default_initializable<iter>);
    static_assert(std::ranges::viewable_range<decltype(a)>);
    static_assert(std::ranges::views::__detail::__can_zip_transform_view<
                  decltype(func1),
                  decltype(a),
                  decltype(b)>);
}

TEST_CASE("Tristate Register Container initialization", "[tri_reg_cont_init]") {
    use_trilogic TriRegisterContainer ct(10, T);
    TriRegisterContainer cf(3, F);
    TriRegisterContainer cx(8, X);

    REQ(ct.size() == 10);
    REQ(cf.size() == 3);
    REQ(cx.size() == 8);

    for (int i = 0; i < 10; ++i) REQ(ct[i] == T);
    for (int i = 0; i < 3; ++i) REQ(cf[i] == F);
    for (int i = 0; i < 8; ++i) REQ(cx[i] == X);

    ct[3] = ct[4] = ct[5] = ct[6] = X;
    REQ(ct[3] == X);
    REQ(ct[4] == X);
    REQ(ct[5] == X);
    REQ(ct[6] == X);
    REQUIRE_THROWS(ct[10]);
    REQUIRE_THROWS(ct[11]);
    REQUIRE_THROWS(ct[-1]);

    REQUIRE_THROWS(ct[123]);

    TriRegisterContainer t(10, T);
    TriRegisterContainer a = t;
    for (int i = 0; i < 10; ++i) {
        INFO(std::format("id: {}", i));
        REQ(t[i] == a[i]);
    }

    TriRegisterContainer b(std::move(a));
    for (int i = 0; i < 10; ++i) REQ(t[i] == b[i]);

    a = t;
    for (int i = 0; i < 10; ++i) { REQ(t[i] == a[i]); }

    b = std::move(a);
    for (int i = 0; i < 10; ++i) REQ(t[i] == b[i]);

    std::string s = "01x01x011xxxx01x011001x01x0111100001x01x011001x01x0110";
    std::string s0 = "00000000000000000000000000";
    std::string s1 = "11111111111111111111111111";
    std::string sx = "xxxxxxxxxxxxxxxxxxxxxxxxxx";

    TriRegisterContainer c(s.begin(), s.end());
    for (int i = 0; i < s.size(); ++i) REQ(c[i] == Tristate(s[i]));

    TriRegisterContainer c0(s0.begin(), s0.end());
    for (int i = 0; i < s0.size(); ++i) REQ(c0[i] == Tristate(s0[i]));

    TriRegisterContainer c1(s1.begin(), s1.end());
    for (int i = 0; i < s1.size(); ++i) REQ(c1[i] == Tristate(s1[i]));

    TriRegisterContainer ccx(sx.begin(), sx.end());
    for (int i = 0; i < sx.size(); ++i) REQ(ccx[i] == Tristate(sx[i]));
}

TEST_CASE("Test Tristate Register data referensing", "[trc_ref]") {
    using TRC = TriRegisterContainer;
    use_trilogic;

    TRC a(10, X);
    TRC::reference a_r1 = *a.begin();
    TRC::reference a_r2 = a[3];
    TRC::reference a_r3(a_r2);

    REQ(a_r1 == a[0]);
    REQ(a_r1 == X);
    REQ(a_r2 == *(a.begin() + 3));
    REQ(a_r2 == a_r3);
    REQ(a_r2 == X);

    a_r2 = T;
    REQ(a_r3 == T);
}

TEST_CASE("Test Tristate Register container iteration", "[tri_reg_cont_iter]") {
    use_trilogic using iter = TriRegisterContainer::iterator;
    using citer = TriRegisterContainer::const_iterator;
    using riter = TriRegisterContainer::reverse_iterator;
    using criter = TriRegisterContainer::const_reverse_iterator;

    TriRegisterContainer a(10, T);
    TriRegisterContainer b(a);
    a[0] = a[2] = a[4] = a[6] = a[8] = F;
    int i;

    iter a_e = a.end();
    REQ(a_e + 1 == a_e);

    riter a_re = a.rend();
    REQ(a_re + 1 == a_re);

    criter a_cre = a.rend();
    REQ(a_cre + 1 == a_cre);

    REQ(a.begin() != b.begin());
    REQ(a.end() != b.end());

    i = 0;
    for (iter it = a.begin(); it != a.end(); ++it, ++i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }

    i = 0;
    for (citer it = a.cbegin(); it != a.cend(); ++it, ++i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }

    i = 0;
    for (citer it = a.begin(); it != a.end(); ++it, ++i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }

    i = 9;
    for (riter it = a.rbegin(); it != a.rend(); ++it, --i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }

    i = 9;
    for (criter it = a.crbegin(); it != a.crend(); ++it, --i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }

    i = 9;
    for (criter it = a.rbegin(); it != a.rend(); ++it, --i) {
        if (i % 2 == 0) REQ(*it == F);
        else REQ(*it == T);
        if (i > 0) {
            REQ(*it == *it);
            REQ(*it != a[i - 1]);
        }
    }
}

TEST_CASE("Null TriRegisterContainer", "trc_null") {
    use_trilogic;

    TriRegisterContainer a;
    TriRegisterContainer b(0, X);

    REQ(a.begin() == a.end());
    REQ(a.cbegin() == a.cend());
    REQ(a.rbegin() == a.rend());
    REQ(a.crbegin() == a.crend());

    REQ(b.begin() == b.end());
    REQ(b.cbegin() == b.cend());
    REQ(b.rbegin() == b.rend());
    REQ(b.crbegin() == b.crend());

    REQUIRE_THROWS(a[0]);
    REQUIRE_THROWS(b[0]);
}
