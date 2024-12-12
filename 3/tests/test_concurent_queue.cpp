#include "concurent-queue.hpp"
#include "lib/container-test-helpers.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <initializer_list>
#include <optional>
#include <sys/types.h>
#include <utility>

TEST_CASE(
    "Tests for concurent_queue methods for composite types",
    "[concurent_queue][concurent_queue_emplace]"
) {
    auto &l = get_init_list<TestCompositeType>();

    SECTION("If copy constructor throws exception") {
        REQUIRE_THROWS([]() {
            ConcurentQueue<UnstableCopyable> r{
                UnstableCopyable("ok1"),
                UnstableCopyable("ok2"),
                UnstableCopyable("fail"),
                UnstableCopyable("ok3")
            };
        }());
    }

    SECTION("If emplace throws exception") {
        ConcurentQueue<UnstableConstructable> r{
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
        ConcurentQueue<UnstableCopyable> r;
        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_NOTHROW(r.emplace("fail"));
        REQUIRE_NOTHROW(r.emplace("ok3"));
    }

    SECTION("If move constructor does not fail for fail-copyable") {
        ConcurentQueue<UnstableCopyable> r;
        r.emplace("ok1");
        r.emplace("ok2");
        r.emplace("fail");
        r.emplace("ok3");

        REQUIRE_NOTHROW([&r]() {
            ConcurentQueue<UnstableCopyable> r1(std::move(r));
        }());
    }

    SECTION("If copy-assgnment operator fails for fail-copyable") {
        ConcurentQueue<UnstableCopyable> r;
        r.emplace("ok1");
        r.emplace("ok2");
        r.emplace("fail");
        r.emplace("ok3");

        ConcurentQueue<UnstableCopyable> r1;
        REQUIRE_THROWS(r1 = r);
    }

    SECTION("Emplace") {
        ConcurentQueue<TestCompositeType> r;

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

    SECTION("Pop if") {
        ConcurentQueue<std::string> q{"asd", "sdf", "dfg", "qwe"};

        REQUIRE(q.pop_if([](auto &a) { return a == "asd"; }) == "asd");
        REQUIRE(q.pop_if([](auto &a) { return a == "asd"; }) == std::nullopt);
        REQUIRE(q.pop_if([](auto &a) { return a == "sdf"; }) == "sdf");
        REQUIRE(q.pop_if([](auto &a) { return a == "sdf"; }) == std::nullopt);
        REQUIRE(q.pop_if([](auto &a) { return a == "dfg"; }) == "dfg");
        REQUIRE(q.pop_if([](auto &a) { return a == "dfg"; }) == std::nullopt);
        REQUIRE(q.pop_if([](auto &a) { return a == "qwe"; }) == "qwe");
        REQUIRE(q.pop_if([](auto &a) { return a == "qwe"; }) == std::nullopt);

        REQUIRE(q.size() == 0);
        REQUIRE(q.empty());
    }

    SECTION("If push(const T &) fails because of fail-copy but queue is still valid") {
        ConcurentQueue<UnstableCopyable> r;

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
        ConcurentQueue<UnstableCopyable> r;

        UnstableCopyable fail("fail");

        REQUIRE_NOTHROW(r.emplace("ok1"));
        REQUIRE_NOTHROW(r.emplace("ok2"));
        REQUIRE_NOTHROW(r.push(std::move(fail)));
        REQUIRE_NOTHROW(r.emplace("ok3"));
    }

    SECTION("If push(T &&) fails because of fail-move but queue is still valid") {
        ConcurentQueue<UnstableMovable> r;

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
}

TEMPLATE_TEST_CASE(
    "Tests for template concurent_queue",
    "[concurent_queue]",
    int,
    TestCompositeType
) {
    auto &l = get_init_list<TestType>();

    SECTION("Defaul Initialization") {
        ConcurentQueue<TestType> r;

        REQUIRE(r.size() == 0);
        REQUIRE(r.empty());
    }

    SECTION("Copy Push") {
        ConcurentQueue<TestType> r;

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
        ConcurentQueue<TestType> r;

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
        ConcurentQueue<TestType> r(l);
        const ConcurentQueue<TestType> &rref = r;

        REQUIRE(r.size() == l.size());
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == *l.begin());
        REQUIRE(r.back() == *(l.begin() + l.size() - 1));

        REQUIRE(rref.front() == r.front());
        REQUIRE(rref.back() == r.back());
    }

    SECTION("Clear") {
        ConcurentQueue<TestType> r(l);
        r.clear();

        REQUIRE(r.size() == 0);
        REQUIRE(r.empty());
    }

    SECTION("Pop") {
        ConcurentQueue<TestType> r(l);

        ssize_t sz = l.size();
        for (auto i : l) {
            if (sz > 1) {
                auto a = r.pop();
                REQUIRE(a == *(l.begin() + l.size() - sz));
                sz -= 1;
                REQUIRE_FALSE(r.empty());
                REQUIRE(r.size() == sz);
                REQUIRE(r.front() == *(l.begin() + l.size() - sz));
                REQUIRE(r.back() == *(l.begin() + (l.size() - 1)));
            } else {
                auto a = r.pop();
                REQUIRE(a == *(l.begin() + l.size() - sz));
                REQUIRE(r.empty());
                REQUIRE(r.size() == 0);
            }
        }

        auto a = r.pop();
        REQUIRE(a == std::nullopt);
        REQUIRE(r.empty());
        REQUIRE(r.size() == 0);
    }

    SECTION("Copy Initialization") {
        ConcurentQueue<TestType> r(l);
        const ConcurentQueue<TestType> &rref = r;
        ConcurentQueue<TestType> r1(rref);
        ConcurentQueue<TestType> r2;
        r2 = rref;
        ConcurentQueue<TestType> r3(l.begin(), l.end());

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

        REQUIRE(r3.size() == l.size());
        REQUIRE_FALSE(r3.empty());

        REQUIRE(r3.front() == *l.begin());
        REQUIRE(r3.back() == *(l.begin() + l.size() - 1));
    }

    SECTION("Move Initialization") {
        ConcurentQueue<TestType> r1(l);
        ConcurentQueue<TestType> r(std::move(r1));

        r1 = ConcurentQueue<TestType>(l);
        ConcurentQueue<TestType> r2 = std::move(r1);

        REQUIRE(r.size() == l.size());
        REQUIRE_FALSE(r.empty());

        REQUIRE(r.front() == *l.begin());
        REQUIRE(r.back() == *(l.begin() + l.size() - 1));

        REQUIRE(r2.size() == l.size());
        REQUIRE_FALSE(r2.empty());

        REQUIRE(r2.front() == *l.begin());
        REQUIRE(r2.back() == *(l.begin() + l.size() - 1));
    }
}
