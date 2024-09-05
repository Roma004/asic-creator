#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <string_view>

namespace IOUtil {
/*!
 * \addtogroup IOUtil
 * Namespace contains methods usefull for various input/output
 * operations with stream objects
 * @{
 */

/**
 * @brief class describing stream eof exception
 */
class eof_exception : public std::exception {
  public:
    virtual char const *what() const noexcept { return "Got EOF!"; }
};

template <class T> using sview = std::basic_string_view<T>;

/**
 * @brief function for skipping characher sequence stored in istream
 * @param in -- stream to check for cahracter
 * @param chr -- character to skip if is at the top of stream
 */
template <class Chr>
static inline void stream_ignore(std::basic_istream<Chr> &in, Chr chr) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), chr);
}

/**
 * @brief function for skipping characher sequence stored in istream
 * @param in -- stream to check for cahracter
 * @param barrier -- list of characters to skip if are at the top of stream
 */
template <class Chr>
static inline void
stream_ignore(std::basic_istream<Chr> &in, const sview<Chr> &barrier) {
    size_t i;
    while ((i = barrier.find(in.peek())) != -1) {
        stream_ignore(in, barrier[i]);
    }
}

/**
 * @brief check wether one of characters specified is at the top of stream
 * @param in -- stream to check for cahracter
 * @param barrier -- list of characters to check
 */
template <class Chr>
static inline bool
is_stream_barrier(std::basic_istream<Chr> &in, const sview<Chr> &barrier) {
    return !in.eof() && barrier.find(in.peek()) != -1;
}

/**
 * @brief extract value from stream, till one of characters specified
 * @param in -- stream to work with
 * @param val -- reference to the container to store value
 * @param barrier -- list of barrier characters
 * @param on_retry -- function called, if extraction was not syccessfull. It may
 * print something to stderr or throw custom exception etc.
 * @throw std::bad_alloc on stream inner allocation issue
 * @throw IOUtil::eof_exception on EOF character got from stream
 */
template <class Chr>
void istream_get(
    std::basic_istream<Chr> &in,
    auto &val,
    const sview<Chr> &barrier,
    std::function<void(void)> on_retry
) {
    while (true) {
        in >> val;
        if (in.eof()) throw eof_exception();
        if (in.bad()) throw std::bad_alloc();
        if (in.fail()) {
            in.clear();
            stream_ignore(in, barrier);
            goto retry;
        }
        if (!is_stream_barrier(in, barrier)) goto retry;
        break;
    retry:
        on_retry();
    }
}

/**
 * @brief extract value from stdin, till one of characters specified
 * @param prompt -- string to print to stdout before reading
 * @param val -- reference to the container to store value
 * @param barrier -- list of barrier characters
 * @throw std::bad_alloc on stdin inner allocation issue
 * @throw IOUtil::eof_exception on EOF character got from stream
 */
void stdin_get(const char *prompt, auto &val, const char *barrier = " \n\t") {
    std::cout << prompt;
    istream_get(std::cin, val, sview<char>(barrier), [=]() {
        std::cout << "Invalid input!\n" << prompt;
    });
}

/**
 * @brief extract value from wstdin, till one of characters specified
 * @param prompt -- string to print to wstdout before reading
 * @param val -- reference to the container to store value
 * @param barrier -- list of barrier characters
 * @throw std::bad_alloc on stdin inner allocation issue
 * @throw IOUtil::eof_exception on EOF character got from stream
 */
void wstdin_get(
    const wchar_t *prompt, auto &val, const wchar_t *barrier = L" \n\t"
) {
    std::wcout << prompt;
    istream_get(std::wcin, val, sview<wchar_t>(barrier), [=]() {
        std::wcout << "Invalid input!\n" << prompt;
    });
}

/*! @} */
}; // namespace IOUtil

