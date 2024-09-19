#include "tri-register.hpp"
#include "tristate.hpp"
#include <algorithm>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

#define use_trilogic          \
    using Tristate::STATE::T; \
    using Tristate::STATE::F; \
    using Tristate::STATE::X; \
    using st = Tristate::STATE;

#define REQ REQUIRE

TEST_CASE(
    "Tristate Register initialization and comparison", "[tri_reg_cmp_init]"
) {
    use_trilogic INFO("initialization tri-register in different ways");

    std::vector<int> asd{0, 1, 5, 0, 1, 1, 0, 8, 0, 1};
    int sdf[] = {0, 1, 5, 0, 1, 1, 0, 8, 0, 1};
    st dfg[] = {F, T, X, F, T, T, F, X, F, T};

    INFO("initialization with default constructor");
    TriRegister reg_null;
    REQUIRE(reg_null.size() == 0);

    INFO("initialization with 0 signs gives null register");
    TriRegister reg_null1(0, T);
    REQUIRE(reg_null1.size() == 0);
    REQUIRE(reg_null1 == reg_null);

    INFO("initialization with n X signs");
    TriRegister reg_x1(6);
    REQUIRE(reg_x1.size() == 6);

    TriRegister reg_t1(6, T);
    REQUIRE(reg_t1.size() == 6);
    REQUIRE(reg_t1 != reg_x1);

    INFO("initialization with n X signs implicit");
    TriRegister reg_x2{X, X, X, X, X, X};
    REQUIRE(reg_x2.size() == 6);

    INFO("initialization with const char *");
    TriRegister reg1("01x0110x01");
    REQUIRE(reg1.size() == 10);

    INFO("initialization with int-initializers");
    TriRegister reg2{0, 1, 5, 0, 1, 1, 0, 8, 0, 1};
    REQUIRE(reg2.size() == 10);

    INFO("initialization with char-initializers");
    TriRegister reg3{'0', 't', 'x', 'f', '1', 't', 'f', 'z', 'F', 'T'};
    REQUIRE(reg3.size() == 10);

    INFO("initialization from abstract iterable type");
    TriRegister reg4(asd.begin(), asd.end());
    REQUIRE(reg4.size() == 10);

    INFO("initialization from int c-array");
    TriRegister reg5(sdf, sdf + 10);
    REQUIRE(reg5.size() == 10);

    INFO("initialization from char c-array");
    TriRegister reg6(dfg, dfg + 10);
    REQUIRE(reg6.size() == 10);

    INFO("Require equal registers are equal");
    REQ(reg_x1 == reg_x2);
    REQ(reg1 == reg2);
    REQ(reg1 == reg3);
    REQ(reg1 == reg4);
    REQ(reg1 == reg5);
    REQ(reg1 == reg6);

    TriRegister reg11("01x0110x00");
    TriRegister reg12("01x0110x0");

    INFO("Require equal-length different registers are not equal");
    REQUIRE(reg1 != reg11);

    INFO("Require partly equal different length registers are not equal");
    REQUIRE(reg1 != reg12);

    INFO("Require different registers are not equal");
    REQ(reg_x1 != reg1);

    INFO("initialization with copy-assignmet operator");
    TriRegister reg13 = reg1;
    REQUIRE(reg13.size() == 10);
    REQUIRE(reg13 == reg1);

    reg13 = reg_x1;
    REQUIRE(reg13.size() == 6);
    REQUIRE(reg13 == reg_x1);

    INFO("initialization with copy constructor");
    TriRegister reg14(reg1);
    REQUIRE(reg14.size() == 10);
    REQUIRE(reg14 == reg1);

    INFO("initialization with move-assignmet operator");
    TriRegister reg7 = reg1;
    TriRegister reg71 = std::move(reg7);
    REQUIRE(reg71.size() == 10);
    REQUIRE(reg71 == reg1);

    INFO("initialization with move constructor");
    reg7 = reg1;
    TriRegister reg72(std::move(reg7));
    REQUIRE(reg72.size() == 10);
    REQUIRE(reg72 == reg1);
}

TEST_CASE("Tristate Register io operators", "[tri_reg_ostream_io]") {
    TriRegister reg("01x0110x01");
    std::stringstream ss, ss1;
    ss << reg;
    REQUIRE(ss.str() == "01x0110x01");

    TriRegister reg1(reg.size()-1);
    ss >> reg1;
    REQUIRE(reg1 == "01x0110x0");
}

TEST_CASE("Iteration over Tristate Register", "[tri_reg_iterate]") {
    std::string s = "01x0110x01";

    std::string rev = s;
    std::ranges::reverse(rev);

    TriRegister reg(s.c_str());

    INFO("Check index iteration");
    for (int i = 0; i < reg.size(); ++i) { REQUIRE(reg[i] == Tristate(s[i])); }

    int i = 0;
    INFO("Check Iterator iteration");
    for (auto it = reg.begin(); it != reg.end(); ++it, ++i) {
        REQUIRE(*it == Tristate(s[i]));
    }

    INFO("Check const Iterator iteration");
    i = 0;
    for (auto it = reg.cbegin(); it != reg.cend(); ++it, ++i) {
        REQUIRE(*it == Tristate(s[i]));
    }

    INFO("Check for reverse iterator");
    i = 0;
    for (auto it = reg.rbegin(); it != reg.rend(); ++it, ++i) {
        REQUIRE(*it == Tristate(rev[i]));
    }

    INFO("Check for const reverse iterator");
    i = 0;
    for (auto it = reg.crbegin(); it != reg.crend(); ++it, ++i) {
        REQUIRE(*it == Tristate(rev[i]));
    }
}

TEST_CASE("Tristate Register binary OR opeartor test", "[tri_reg_or]") {
    use_trilogic

    TriRegister a = "10010x01";
    TriRegister bt(a.size(), T);
    TriRegister bf(a.size(), F);
    TriRegister bx(a.size(), X);

    TriRegister ct(3, T);
    TriRegister cf(3, F);
    TriRegister cx(3, X);

    TriRegister tmp;

    INFO("check rule a | a == a");
    REQUIRE((a | a) == a);
    tmp = a; tmp |= a; REQUIRE(tmp == a);

    INFO("check rule a | 1 == 1");
    REQUIRE((a | bt) == bt);
    tmp = a; tmp |= bt; REQUIRE(tmp == bt);

    INFO("check rule a | 0 == a");
    REQUIRE((a | bf) == a);
    tmp = a; tmp |= bf; REQUIRE(tmp == a);

    INFO("check rule a | x == {c : c = (1 if i == 1 else x) for i in a}");
    REQUIRE((a | bx) == "1xx1xxx1");
    tmp = a; tmp |= bx; REQUIRE(tmp == "1xx1xxx1");

    INFO("check rule 1 | 1 = 1");
    REQUIRE((bt | bt) == bt);
    tmp = bt; tmp |= bt; REQUIRE(tmp == bt);

    INFO("check rule 1 | 0 = 1");
    REQUIRE((bt | bf) == bt);
    tmp = bt; tmp |= bf; REQUIRE(tmp == bt);

    INFO("check rule 0 | 0 = 0");
    REQUIRE((bf | bf) == bf);
    tmp = bf; tmp |= bf; REQUIRE(tmp == bf);

    INFO("check rule 1 | x = 1");
    REQUIRE((bt | bx) == bt);
    tmp = bt; tmp |= bx; REQUIRE(tmp == bt);

    INFO("check rule 0 | x = x");
    REQUIRE((bf | bx) == bx);
    tmp = bf; tmp |= bx; REQUIRE(tmp == bx);

    INFO("check rule x | x = x");
    REQUIRE((bx | bx) == bx);
    tmp = bx; tmp |= bx; REQUIRE(tmp == bx);

    INFO("check for operation with different-length registers");
    tmp = bt; REQUIRE_THROWS(tmp |= cx);
    REQUIRE_THROWS((bt | cx) == "111");
    try {
        TriRegister asd = bt | cx;
    } catch (const TriRegister::reg_length_error &e) {
        std::string what = e.what();
        REQUIRE(
            what
            == "Binary operation `|` between reqisters of lengths 8 and 3 "
               "is not a valid operation"
        );
    }
}

TEST_CASE("Tristate Register binary AND opeartor test", "[tri_reg_and]") {
    use_trilogic

    TriRegister a = "10010x01";
    TriRegister bt(a.size(), T);
    TriRegister bf(a.size(), F);
    TriRegister bx(a.size(), X);

    TriRegister ct(3, T);
    TriRegister cf(3, F);
    TriRegister cx(3, X);
    TriRegister tmp;

    INFO("check rule a & a == a");
    tmp = a; tmp &= a; REQUIRE(tmp == a);
    REQUIRE((a & a) == a);

    INFO("check rule a & 1 == a");
    tmp = a; tmp &= bt; REQUIRE(tmp == a);
    REQUIRE((a & bt) == a);

    INFO("check rule a & 0 == 0");
    tmp = a; tmp &= bf; REQUIRE(tmp == bf);
    REQUIRE((a & bf) == bf);

    INFO("check rule a & x == {c : c = (0 if i == 0 else x) for i in a}");
    tmp = a; tmp &= bx; REQUIRE(tmp == "x00x0x0x");
    REQUIRE((a & bx) == "x00x0x0x");

    INFO("check rule 1 & 1 = 1");
    tmp = bt; tmp &= bt; REQUIRE(tmp == bt);
    REQUIRE((bt & bt) == bt);

    INFO("check rule 1 & 0 = 0");
    tmp = bt; tmp &= bf; REQUIRE(tmp == bf);
    REQUIRE((bt & bf) == bf);

    INFO("check rule 0 & 0 = 0");
    tmp = bf; tmp &= bf; REQUIRE(tmp == bf);
    REQUIRE((bf & bf) == bf);

    INFO("check rule 1 & x = x");
    tmp = bt; tmp &= bx; REQUIRE(tmp == bx);
    REQUIRE((bt & bx) == bx);

    INFO("check rule 0 & x = 0");
    tmp = bf; tmp &= bx; REQUIRE(tmp == bf);
    REQUIRE((bf & bx) == bf);

    INFO("check rule x & x = x");
    tmp = bx; tmp &= bx; REQUIRE(tmp == bx);
    REQUIRE((bx & bx) == bx);

    INFO("check for operation with different-length registers");
    tmp = bt; REQUIRE_THROWS(tmp &= cx);
    REQUIRE_THROWS(bt & cx);
    try {
        TriRegister asd = bt & cx;
    } catch (const TriRegister::reg_length_error &e) {
        std::string what = e.what();
        REQUIRE(
            what
            == "Binary operation `&` between reqisters of lengths 8 and 3 "
               "is not a valid operation"
        );
    }
}

TEST_CASE("Tristate Register binary NOT opeartor test", "[tri_reg_not]") {
    use_trilogic

    TriRegister a = "10010x01";
    TriRegister bt(a.size(), T);
    TriRegister bf(a.size(), F);
    TriRegister bx(a.size(), X);

    INFO("check rule ~a == {c : c = (i: 0=>1, 1=>0, x=>x) for i in a}");
    REQUIRE(~a == "01101x10");

    INFO("check rule ~1 = 0");
    REQUIRE(~bt == bf);

    INFO("check rule ~0 = 1");
    REQUIRE(~bf == bt);

    INFO("check rule ~x = x");
    REQUIRE(~bx == bx);
}

TEST_CASE("Tristate Register is_fully_defined", "[tri_reg_check_def]") {
    use_trilogic

    TriRegister a = "10010x01";
    TriRegister b(a.size(), T);
    TriRegister c(a.size(), F);
    TriRegister d(a.size(), X);

    INFO("check 1 is fully defined");
    REQUIRE(b.is_fully_defined());

    INFO("check 0 is fully defined");
    REQUIRE(c.is_fully_defined());

    INFO("check x is not fully defined");
    REQUIRE(!d.is_fully_defined());

    INFO("check 10010x01 is not fully defined");
    REQUIRE(!a.is_fully_defined());
}

TEST_CASE("Tristate Register extendings", "[tri_reg_ext]") {
    use_trilogic

    TriRegister t(3, T);
    TriRegister f(4, F);
    TriRegister x(5, X);

    INFO("check 0000 (+) 111 = 0000111");
    TriRegister a = t; a.extend_left(f);
    REQUIRE(a == "0000111");

    INFO("check 0000 (+) xxxxx = 0000xxxx");
    TriRegister b = f; b.extend_right(x);
    REQUIRE(b == "0000xxxxx");
}
