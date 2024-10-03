#pragma once
#include <memory>
#include <vector>

template <class T>
class Factory {
public:
    Factory() = default;
    Factory(const Factory &) = delete;
    Factory(Factory &&) = delete;

    Factory &operator=(const Factory &) = delete;
    Factory &operator=(Factory &&) = delete;

    template <class... Args>
    T &make(Args &&... args) {
        items.emplace_back(std::make_unique<T>(args...));
        return *items.back();
    }

    std::vector<std::unique_ptr<T>> items;
};
