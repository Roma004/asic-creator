#include "syllables_divider.h"
#include <algorithm>
#include <format>
#include <stdexcept>

namespace {
/** \brief Check if letter `c` is in alphabeth `str` */
static inline bool is_in_alp(const std::wstring &str, wchar_t c) {
    return str.find(c) != -1;
}

/** \brief Check if letter is vowel */
static inline bool is_vowel(wchar_t c) {
    return is_in_alp(L"АЕЁИОУЭЮЯаеёиоуыэюя", c);
}

/** \brief Check if letter is unpaired consonant */
static inline bool is_unpaired(wchar_t c) {
    return is_in_alp(L"ЙЛМРНйлмнр", c);
}

/** \brief Check if letter is in russian alphabeth */
static inline bool is_russian(wchar_t c) {
    return is_in_alp(
        L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя", c
    );
}

/** \brief Check if letter represents a special sign and need to be handled in
 * different way than others */
static inline bool is_special(wchar_t c) { return is_in_alp(L"Ййъь", c); }

/** \brief Check if the word given matches the rules of russian language */
static inline void check_word(std::wstring str) {
    for (int i = 0; i < str.size(); ++i) {
        wchar_t ch = str[i];
        if (!is_russian(ch)) {
            throw std::runtime_error(
                std::format("Invalid character found at position {}", i)
            );
        }

        if (i + 1 < str.size() && is_special(str[i])
            && is_special(str[i + 1])) {
            throw std::runtime_error(std::format(
                "Sequence of letters at position {} not permited "
                "by rules of Russian language",
                i
            ));
        }
    }
}

}; // namespace

const wchar_t *SyllablesDivider::next_syll_end(const wchar_t *ptr) const {
    if ((ptr = std::find_if(ptr, strend, is_vowel)) == strend) return strend;

    ptr += 1;
    for (; *ptr; ++ptr) {
        wchar_t c = *ptr, c1 = *(ptr + 1);
        if (!is_special(c) && (!is_unpaired(c) || is_vowel(c1) || c1 == c))
            break;
    }

    if (std::find_if(ptr, strend, is_vowel) != strend) return ptr;
    return strend;
}

SyllablesDivider::SyllablesDivider(const wchar_t *s) {
    size_t len = wcslen(s);
    check_word(std::wstring(s));
    str = new wchar_t[len + 1];
    std::copy(s, s + len + 1, str);
    strend = str + len;
}

SyllablesDivider::SyllablesDivider(const std::wstring &s) :
    SyllablesDivider(s.c_str()) {}

SyllablesDivider::SyllablesDivider(const SyllablesDivider &sd) :
    SyllablesDivider(sd.str) {}

SyllablesDivider::~SyllablesDivider() { delete[] str; }

std::wstring SyllablesDivider::get_syll(const wchar_t *&ptr) const {
    const wchar_t *syll_end = next_syll_end(ptr);
    if (syll_end == ptr) {
        throw std::runtime_error("End of syllables sequence");
    }
    std::wstring res(ptr, syll_end);
    ptr = syll_end;
    return res;
}

SyllablesDivider::iterator SyllablesDivider::begin() const {
    return iterator(*this, str);
}

SyllablesDivider::iterator SyllablesDivider::end() const {
    return iterator(*this, nullptr);
}

SyllablesDivider::iterator::iterator(
    const SyllablesDivider &d, const wchar_t *ptr
) : d(d), ptr(ptr) {
    if (this->ptr != nullptr) buf = d.get_syll(this->ptr);
}

const std::wstring &SyllablesDivider::iterator::operator*() {
    if (ptr != nullptr) return buf;
    throw std::runtime_error("End of syllables sequence");
}

bool SyllablesDivider::iterator::operator!=(const iterator &o) {
    return !(*this == o);
}
bool SyllablesDivider::iterator::operator==(const iterator &o) {
    return o.ptr == ptr;
}

SyllablesDivider::iterator &SyllablesDivider::iterator::operator++() {
    if (ptr == nullptr)
        throw std::runtime_error("Trying to increment end iterator");
    try {
        buf = d.get_syll(ptr);
    } catch (const std::runtime_error &e) { ptr = nullptr; }
    return *this;
}
