#include <sstream>
#include <string>

#include "split.h"
#include "syllables_divider.h"

static inline std::wstring get_sylled_word(SyllablesDivider &sd) {
    std::wstringstream ss;
    bool first_syll = true;
    for (auto &syll : sd) {
        if (!first_syll) ss << '-';
        ss << syll;
        first_syll = false;
    }
    return ss.str();
}

std::wstring word_syll_split(const std::wstring &str) {
    SyllablesDivider sd(str);
    return get_sylled_word(sd);
}

std::wstring word_syll_split(const wchar_t *str) {
    SyllablesDivider sd(str);
    return get_sylled_word(sd);
}
