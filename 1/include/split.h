#pragma once

/**
 * @file
 * File contains functions for syllabifiction of strings
 * */

/*!
 * \addtogroup TaskSources
 * @{
 * */

#include <string>

/**
 * @fn std::wstring word_syll_split(const std::wstring &str)
 * Function for syllabification of given word
 *
 * @param str -- string containing word to syllabify
 *
 * @throw std::runtime_error containing the message of the error occured
 * @throw std::bad_alloc
 */
std::wstring word_syll_split(const std::wstring &str);

/**
 * @overload std::wstring word_syll_split(const wchar_t *str)
 * Function for syllabification of given word (overload with wchar_t cstring)
 *
 * @param str -- string containing word to syllabify
 *
 * @throw std::runtime_error containing the message of the error occured
 * @throw std::bad_alloc
 */
std::wstring word_syll_split(const wchar_t *str);

/*! @} */
