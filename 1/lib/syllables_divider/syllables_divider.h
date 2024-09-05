#pragma once
#include <string>

/**
 * \brief Class for phonetical syllabification of russian words.
 *
 * Provides the way to iterate over the syllables in the given word splited
 * according to the rules of russian phonetics.
 *
 * See [the target
 * specification](https://russkiiyazyk.ru/fonetika/delenie-slov-na-slogi.html),
 * on which thich engine bases.
 *
 * Usecase:
 * ```
 * wstring str = L"абракадабра";
 * SyllablesDivider sd(str);
 * for (auto syll : sd) std::wcout << *syll << " ";
 * std::wcout << std::endl;
 * ```
 * the output:
 * ```
 * a бра ка да бра
 * ```
 */
class SyllablesDivider {
    wchar_t *str;
    const wchar_t *strend;

  protected:
    /**
     * @protected
     * @brief Get pointer to syllable started at `ptr`
     * @param ptr -- the start position of the syllable
     *
     * @return pointer to the end of syllable
     * */
    const wchar_t *next_syll_end(const wchar_t *ptr) const;

    /**
     * @protected
     * @brief Get syllable started at ptr as std::wstring and overwrite ptr
     * with the end of syllable
     *
     * @param ptr -- the start position of the syllable. Not changes, if the
     *                call was not successfull
     *
     * @return std::wstring containig the syllable
     * @throw std::runtime_error "End of syllables sequence" if ptr == strend
     * */
    std::wstring get_syll(const wchar_t *&ptr) const;

  public:
    /** \brief class for iteration over SyllablesDivider */
    class iterator {
        const SyllablesDivider &d;
        const wchar_t *ptr;
        std::wstring buf;

      public:
        iterator() = delete;
        iterator(const SyllablesDivider &d, const wchar_t *ptr);
        iterator(const iterator &) = default;
        iterator(iterator &&) = default;

        const std::wstring &operator*();
        bool operator!=(const iterator &o);
        bool operator==(const iterator &o);

        iterator &operator++();
    };
    friend class iterator;

    SyllablesDivider() = delete;
    SyllablesDivider(const wchar_t *);
    SyllablesDivider(const std::wstring &);

    SyllablesDivider(const SyllablesDivider &);
    SyllablesDivider(SyllablesDivider &&) = delete;

    ~SyllablesDivider();

    /**
     * \brief Get iterator to the first syllable in word
     */
    iterator begin() const;
    /**
     * \brief Get iterator to the end of syllable iteartion
     */
    iterator end() const;
};

