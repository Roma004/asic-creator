#pragma once
#include <format>
#include <iostream>
#include <mutex>
#include <ostream>

class Logger {
    std::mutex m;
    std::string name;
    std::ostream &stream;

  public:
    Logger(std::string_view name, std::ostream &stream) :
        name(name), stream(stream) {}

    template <class T> Logger &operator<<(const T &t) noexcept {
        std::unique_lock l(m);
        stream << t;
        return *this;
    }

    template <class... Args>
    void write(std::string_view fmt, Args &&...args) {
        std::unique_lock l(m);
        stream << std::format("{}: ", name)
               << std::vformat(fmt, std::make_format_args(args...))
               << std::endl;
    }
};

extern Logger debug;
extern Logger error;
extern Logger info;

