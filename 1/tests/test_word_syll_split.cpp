#include <catch2/catch_test_macros.hpp>
#include <split.h>

TEST_CASE("Splitting words into syllables", "[word_syll_split]") {
    REQUIRE(word_syll_split(L"ф") == L"ф");
    REQUIRE(word_syll_split(L"я") == L"я");
    REQUIRE(word_syll_split(L"фыва") == L"фы-ва");
    REQUIRE(word_syll_split(L"Силлабификация") == L"Си-лла-би-фи-ка-ци-я");

    REQUIRE(word_syll_split(std::wstring(L"ф")) == L"ф");
    REQUIRE(word_syll_split(std::wstring(L"я")) == L"я");
    REQUIRE(word_syll_split(std::wstring(L"фыва")) == L"фы-ва");
    REQUIRE(word_syll_split(std::wstring(L"Силлабификация")) == L"Си-лла-би-фи-ка-ци-я");
}
