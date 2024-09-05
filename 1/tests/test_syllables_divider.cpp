#include "syllables_divider.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <string>

std::wstring get_nth_syll_with_sd(SyllablesDivider &sd, int n) {
    for (auto &syll : sd) {
        if (n == 0) return syll;
        n -= 1;
    }
    throw std::runtime_error("Syllables are left");
}

std::wstring get_nth_syll(const wchar_t *str, int n) {
    SyllablesDivider sd(str);
    return get_nth_syll_with_sd(sd, n);
}

std::wstring get_nth_syll_wstring(const std::wstring &str, int n) {
    SyllablesDivider sd(str);
    return get_nth_syll_with_sd(sd, n);
}

std::wstring get_nth_syll_copy(const std::wstring &str, int n) {
    SyllablesDivider sd(str);
    SyllablesDivider sd1 = sd;
    return get_nth_syll_with_sd(sd1, n);
}

auto can_dereference_strend() {
    SyllablesDivider sd(L"фывапролд");
    auto end_id = sd.end();
    auto syll = *end_id;
    return syll;
}

bool can_overstep_strend() {
    SyllablesDivider sd(L"фывапролд");
    auto end_id = sd.end();
    ++end_id;
    return end_id == sd.end();
}

#define GET_NTH_SYLL_VARIANT(func) \
    REQUIRE(func(L"Силлабификация", 0) == L"Си"); \
    REQUIRE(func(L"Силлабификация", 5) == L"ци"); \
    REQUIRE(func(L"Силлабификация", 6) == L"я"); \
    REQUIRE_THROWS(func(L"Силлабификация", 7)); \
    REQUIRE_THROWS(func(L"Силл-абификация", 0)); \
    REQUIRE_THROWS(func(L"Силлабъъификация", 0));

TEST_CASE("SyllablesDivider functionality", "[syllables_divider]") {
    GET_NTH_SYLL_VARIANT(get_nth_syll)
    GET_NTH_SYLL_VARIANT(get_nth_syll_wstring)
    GET_NTH_SYLL_VARIANT(get_nth_syll_copy)

    REQUIRE_THROWS(can_dereference_strend());
    REQUIRE_THROWS(can_overstep_strend());
}
