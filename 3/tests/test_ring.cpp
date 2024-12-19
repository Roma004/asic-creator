#include "ring.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <initializer_list>
#include <ranges>
#include <sys/types.h>
#include <utility>
#include "lib/container-test-helpers.hpp"

TEST_CASE(
    "Tests for ring methods for composite types", "[ring][ring_emplace]"
) {
    auto &l = get_init_list<TestCompositeType>();

    SECTION("If copy constructor throws exception") {
        REQUIRE_THROWS([]() {
            Ring<UnstableCopyable> r{
                UnstableCopyable("ok1"),
                UnstableCopyable("ok2"),
                UnstableCopyable("fail"),
                UnstableCopyable("ok3")
            };
        }());
    }

    SECTION("If emplace throws exception") {
        Ring<UnstableConstructable> r{
            UnstableConstructable("ok1"),
            UnstableConstructable("ok2"),
            UnstableConstructable("ok3")
        };

        REQUIRE_THROWS(r.emplace("fail"));

        REQUIRE(r.size() == 3);
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.back() == UnstableConstructable("ok3"));
    }

    SECTION("If emplace does not throw exception for fail-copyable") {
        Ring<UnstableCopyable> r;
        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_NOTHROW(r.emplace("fail"));
        REQUIRE_NOTHROW(r.emplace("ok3"));
    }

    SECTION("If move constructor does not fail for fail-copyable") {
        Ring<UnstableCopyable> r;
        r.emplace("ok1");
        r.emplace("ok2");
        r.emplace("fail");
        r.emplace("ok3");

        REQUIRE_NOTHROW([&r]() {
            Ring<UnstableCopyable> r1(std::move(r));
        }());
    }

    SECTION("If push(const T &) fails because of fail-copy but queue is still valid") {
        Ring<UnstableCopyable> r;

        UnstableCopyable fail("fail");

        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_THROWS(r.push(fail));

        REQUIRE(r.size() == 2);
        REQUIRE(r.front() == UnstableCopyable("ok1"));
        REQUIRE(r.back() == UnstableCopyable("ok2"));

        REQUIRE_NOTHROW(r.emplace("ok3"));

        REQUIRE(r.size() == 3);
        REQUIRE(r.front() == UnstableCopyable("ok1"));
        REQUIRE(r.back() == UnstableCopyable("ok3"));
    }

    SECTION("If push(T &&) does not fail for fail-copy object") {
        Ring<UnstableCopyable> r;

        UnstableCopyable fail("fail");

        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_NOTHROW(r.push(std::move(fail)));
        REQUIRE_NOTHROW(r.emplace("ok3"));
    }

    SECTION("If push(T &&) fails because of fail-move but queue is still valid") {
        Ring<UnstableMovable> r;

        UnstableMovable fail("fail");

        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_THROWS(r.push(std::move(fail)));

        REQUIRE(r.size() == 2);
        REQUIRE(r.front() == UnstableMovable("ok1"));
        REQUIRE(r.back() == UnstableMovable("ok2"));

        REQUIRE_NOTHROW(r.emplace("ok3"));

        REQUIRE(r.size() == 3);
        REQUIRE(r.front() == UnstableMovable("ok1"));
        REQUIRE(r.back() == UnstableMovable("ok3"));
    }

    SECTION("Emplace") {
        Ring<TestCompositeType> r;

        TestCompositeType v1("asd", 3);
        TestCompositeType v2;
        TestCompositeType v3("sdf", std::initializer_list<int>{1, 2, 3});

        r.emplace("asd", 3);
        r.emplace();

        REQUIRE(r.size() == 2);
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == v1);
        REQUIRE(r.back() == v2);

        r.emplace("sdf", std::initializer_list<int>{1, 2, 3});

        REQUIRE(r.size() == 3);
        REQUIRE(r.back() == v3);
    }

    SECTION("Iterator") {
        Ring<TestCompositeType> r(l);

        SECTION("operator ->") { REQUIRE(r.begin()->size() == 4); }
        SECTION("copy constructor") {
            Ring<TestCompositeType>::iterator it(r.begin());
            Ring<TestCompositeType>::const_iterator itc(it);
            REQUIRE(it == r.begin());
            REQUIRE(itc == r.begin());
        }
        SECTION("copy assignment operator") {
            Ring<TestCompositeType>::iterator it;
            it = r.begin();
            Ring<TestCompositeType>::const_iterator itc;
            itc = it;
            REQUIRE(it == r.begin());
            REQUIRE(itc == r.begin());
        }
    }
}

TEMPLATE_TEST_CASE(
    "Tests for template ring", "[ring]", int, TestCompositeType
) {
    auto &l = get_init_list<TestType>();

    SECTION("Defaul Initialization") {
        Ring<TestType> r;

        REQUIRE(r.size() == 0);
        REQUIRE(r.empty());
    }

    SECTION("Copy Push") {
        Ring<TestType> r;

        size_t sz = 0;
        for (auto &i : l) {
            r.push(i);
            sz += 1;
            REQUIRE_FALSE(r.empty());
            REQUIRE(r.size() == sz);
            REQUIRE(r.back() == i);
            REQUIRE(r.front() == *l.begin());
        }
    }

    SECTION("Move Push") {
        Ring<TestType> r;

        size_t sz = 0;
        for (auto i : l) {
            r.push(std::move(i));
            sz += 1;
            REQUIRE_FALSE(r.empty());
            REQUIRE(r.size() == sz);
            REQUIRE(r.back() == i);
            REQUIRE(r.front() == *l.begin());
        }
    }

    SECTION("Initializer list Initialization") {
        Ring<TestType> r(l);
        const Ring<TestType> &rref = r;

        REQUIRE(r.size() == l.size());
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == *l.begin());
        REQUIRE(r.back() == *(l.begin() + l.size() - 1));

        REQUIRE(rref.front() == r.front());
        REQUIRE(rref.back() == r.back());
    }

    SECTION("Clear") {
        Ring<TestType> r(l);
        r.clear();

        REQUIRE(r.size() == 0);
        REQUIRE(r.empty());
    }

    SECTION("Pop") {
        Ring<TestType> r(l);

        ssize_t sz = l.size();
        for (auto i : l) {
            if (sz > 1) {
                r.pop();
                sz -= 1;
                REQUIRE_FALSE(r.empty());
                REQUIRE(r.size() == sz);
                REQUIRE(r.front() == *(l.begin() + l.size() - sz));
                REQUIRE(r.back() == *(l.begin() + (l.size() - 1)));
            } else {
                r.pop();
                REQUIRE(r.empty());
                REQUIRE(r.size() == 0);
            }
        }

        r.pop();
        REQUIRE(r.empty());
        REQUIRE(r.size() == 0);
    }

    SECTION("Copy Initialization") {
        Ring<TestType> r(l);
        const Ring<TestType> &rref = r;
        Ring<TestType> r1(rref);
        Ring<TestType> r2;
        r2 = rref;

        REQUIRE(r.size() == l.size());
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == *l.begin());
        REQUIRE(r.back() == *(l.begin() + l.size() - 1));

        REQUIRE(r1.size() == l.size());
        REQUIRE_FALSE(r1.empty());

        REQUIRE(r1.front() == *l.begin());
        REQUIRE(r1.back() == *(l.begin() + l.size() - 1));

        REQUIRE(r2.size() == l.size());
        REQUIRE_FALSE(r2.empty());

        REQUIRE(r2.front() == *l.begin());
        REQUIRE(r2.back() == *(l.begin() + l.size() - 1));
    }

    SECTION("Move Initialization") {
        Ring<TestType> r1(l);
        Ring<TestType> r(std::move(r1));

        r1 = Ring<TestType>(l);
        Ring<TestType> r2 = std::move(r1);

        REQUIRE(r.size() == l.size());
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == *l.begin());
        REQUIRE(r.back() == *(l.begin() + l.size() - 1));

        REQUIRE(r2.size() == l.size());
        REQUIRE_FALSE(r2.empty());

        REQUIRE(r2.front() == *l.begin());
        REQUIRE(r2.back() == *(l.begin() + l.size() - 1));
    }

    SECTION("Iteration") {
        Ring<TestType> r(l);
        const Ring<TestType> &r1 = r;

        for (auto &&[a, b] : std::ranges::zip_view(l, r)) REQUIRE(a == b);
        for (auto &&[a, b] : std::ranges::zip_view(l, r1)) REQUIRE(a == b);

        auto b1 = r.begin();
        auto b2 = r.begin();
        auto bc1 = r1.begin();
        auto bc2 = r1.begin();

        REQUIRE(b1 == b2);
        REQUIRE(bc1 == bc2);
        REQUIRE(b1 == bc2);

        auto a1 = b1++;

        REQUIRE(a1 == b2);
        REQUIRE(*b1 == *(l.begin() + 1));
    }
}
