#include "tristate.hpp"
#include <catch2/catch_test_macros.hpp>

#define use_trilogic \
    using Tristate::STATE::T; \
    using Tristate::STATE::F; \
    using Tristate::STATE::X;

#define REQ REQUIRE

TEST_CASE("Tristate initialization and comparison", "[tristate_cmp_init]") {
    use_trilogic
    using ST = Tristate::STATE;

    Tristate def;
    Tristate t1(T),   f1(F),   x1(X);
    Tristate t2('T'), f2('F'), x2('X');
    Tristate t3('t'), f3('f'), x3('a');
    Tristate t4('1'), f4('0');
    Tristate t5(1), f5(0), x5(123);
    Tristate changing;

    INFO("check all initialization variants gives the same result");
    REQ(t1 == t2); REQ(t1 == t3); REQ(t1 == t4); REQ(t1 == t5);
    REQ(f1 == f2); REQ(f1 == f3); REQ(f1 == f4); REQ(f1 == f5);
    REQ(x1 == x2); REQ(x1 == x3); REQ(x1 == x5); REQ(x1 == def);

    INFO("check explicit operators work");
    REQ((char)t1 == '1'); REQ((char)f1 == '0'); REQ((char)x1 == 'x');
    REQ((ST)t1 == T); REQ((ST)f1 == F); REQ((ST)x1 == X);

    INFO("check comparison with STATE works");
    REQ(t1 == T); REQ(f1 == F); REQ(x1 == X);

    INFO("check right comparison with state works");
    REQ(T == t1); REQ(F == f1); REQ(X == x1);

    INFO("check if tristate-assignment operator works");
    changing = t1;
    REQ(changing == t1); REQ(changing == T);

    INFO("check if StATE-assignment operator works");
    changing = F;
    REQ(changing == f1); REQ(changing == F);

    INFO("check if int-assignment operator works");
    changing = 1;
    REQ(changing == T);

    INFO("check if char-assignment operator works");
    changing = 'Q';
    REQ(changing == X);
}

TEST_CASE("Tristate IO to std::stream", "[tristate_io]") {
    use_trilogic
    Tristate a = T, b = F, c = X;
    std::stringstream ss;
    ss << a << b << c;
    REQ(ss.str() == "10x");

    Tristate a1, b1, c1;
    ss >> a1 >> b1 >> c1;
    REQ(a == a1); REQ(a1 == T);
    REQ(b == b1); REQ(b1 == F);
    REQ(c == c1); REQ(c1 == X);
}

TEST_CASE("Tristate logical and binary AND", "[tristate_and]") {
    use_trilogic

    Tristate f(0), t(1), x('x');

    INFO("check for logical AND for Trstate-Tristate pair");
    REQ((t && t) == t); REQ((t && f) == f); REQ((t && x) == x);
    REQ((f && t) == f); REQ((f && f) == f); REQ((f && x) == f);
    REQ((x && t) == x); REQ((x && f) == f); REQ((x && x) == x);

    INFO("check for binary AND for Trstate-Tristate pair");
    REQ((t & t) == t); REQ((t & f) == f); REQ((t & x) == x);
    REQ((f & t) == f); REQ((f & f) == f); REQ((f & x) == f);
    REQ((x & t) == x); REQ((x & f) == f); REQ((x & x) == x);

    INFO("check for logical AND for Trstate-STATE pair");
    REQ((t && T) == T); REQ((t && F) == F); REQ((t && X) == X);
    REQ((f && T) == F); REQ((f && F) == F); REQ((f && X) == F);
    REQ((x && T) == X); REQ((x && F) == F); REQ((x && X) == X);

    INFO("check for binary AND for Trstate-STATE pair");
    REQ((t & T) == T); REQ((t & F) == F); REQ((t & X) == X);
    REQ((f & T) == F); REQ((f & F) == F); REQ((f & X) == F);
    REQ((x & T) == X); REQ((x & F) == F); REQ((x & X) == X);

    INFO("check for logical AND for STATE-STATE pair");
    REQ((t && T) == T); REQ((t && F) == F); REQ((t && X) == X);
    REQ((f && T) == F); REQ((f && F) == F); REQ((f && X) == F);
    REQ((x && T) == X); REQ((x && F) == F); REQ((x && X) == X);

    INFO("check for binary AND for STATE-STATE pair");
    REQ((t & T) == T); REQ((t & F) == F); REQ((t & X) == X);
    REQ((f & T) == F); REQ((f & F) == F); REQ((f & X) == F);
    REQ((x & T) == X); REQ((x & F) == F); REQ((x & X) == X);
}

TEST_CASE("Tristate logical and binary OR", "[tristate_or]") {
    use_trilogic

    Tristate f(0), t(1), x('x');

    INFO("check for logical OR for Trstate-Tristate pair");
    REQ((t || t) == t); REQ((t || f) == t); REQ((t || x) == t);
    REQ((f || t) == t); REQ((f || f) == f); REQ((f || x) == x);
    REQ((x || t) == t); REQ((x || f) == x); REQ((x || x) == x);

    INFO("check for binary OR for Trstate-Tristate pair");
    REQ((t | t) == t); REQ((t | f) == t); REQ((t | x) == t);
    REQ((f | t) == t); REQ((f | f) == f); REQ((f | x) == x);
    REQ((x | t) == t); REQ((x | f) == x); REQ((x | x) == x);

    INFO("check for logical OR for Trstate-STATE pair");
    REQ((t || T) == T); REQ((t || F) == T); REQ((t || X) == T);
    REQ((f || T) == T); REQ((f || F) == F); REQ((f || X) == X);
    REQ((x || T) == T); REQ((x || F) == X); REQ((x || X) == X);

    INFO("check for binary OR for Trstate-STATE pair");
    REQ((t | T) == T); REQ((t | F) == T); REQ((t | X) == T);
    REQ((f | T) == T); REQ((f | F) == F); REQ((f | X) == X);
    REQ((x | T) == T); REQ((x | F) == X); REQ((x | X) == X);

    INFO("check for logical OR for STATE-STATE pair");
    REQ((T || T) == T); REQ((T || F) == T); REQ((T || X) == T);
    REQ((F || T) == T); REQ((F || F) == F); REQ((F || X) == X);
    REQ((X || T) == T); REQ((X || F) == X); REQ((X || X) == X);

    INFO("check for binary OR for STATE-STATE pair");
    REQ((T | T) == T); REQ((T | F) == T); REQ((T | X) == T);
    REQ((F | T) == T); REQ((F | F) == F); REQ((F | X) == X);
    REQ((X | T) == T); REQ((X | F) == X); REQ((X | X) == X);
}

TEST_CASE("Tristate logical and binary NOT", "[tristate_not]") {
    use_trilogic

    Tristate f(0), t(1), x('x');

    INFO("check for logical NOT for Trstate");
    REQ(!t == f); REQ(!f == t); REQ(!x == x);

    INFO("check for binary NOT for Trstate");
    REQ(~t == f); REQ(~f == t); REQ(~x == x);

    INFO("check for logical NOT for STATE");
    REQ(!T == F); REQ(!F == T); REQ(!X == X);

    INFO("check for binary NOT for STATE");
    REQ(~T == F); REQ(~F == T); REQ(~X == X);
}
