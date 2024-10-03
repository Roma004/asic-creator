#pragma once

#include <functional>
#include <mutex>
#include <queue>

template <typename T> class ThreadSafeQueue {
  private:
    std::queue<T> q;
    std::mutex &mutex;

  public:
    ThreadSafeQueue() = delete;
    explicit ThreadSafeQueue(std::mutex &m) : mutex(m), q() {}
    ThreadSafeQueue(const ThreadSafeQueue &) = default;
    ThreadSafeQueue(ThreadSafeQueue &&) = default;

    bool empty() const {
        std::unique_lock<std::mutex> lock(mutex);
        return q.empty();
    }

    void push(const std::queue<T>::value_type &item) {
        std::unique_lock<std::mutex> lock(mutex);
        q.push(item);
    }

    void push(std::queue<T>::value_type &&item) {
        std::unique_lock<std::mutex> lock(mutex);
        q.push(std::move(item));
    }

    constexpr size_t size() const noexcept { return q.size(); }

    std::queue<T>::reference front() {
        std::unique_lock<std::mutex> lock(mutex);
        return q.front();
    }

    std::queue<T>::const_reference front() const {
        std::unique_lock<std::mutex> lock(mutex);
        return q.front();
    }

    std::queue<T>::reference back() {
        std::unique_lock<std::mutex> lock(mutex);
        return q.back();
    }

    std::queue<T>::const_reference back() const {
        std::unique_lock<std::mutex> lock(mutex);
        return q.back();
    }

    bool pop(std::queue<T>::value_type &res) {
        return pop_if(res, [](auto) { return true; });
    }

    bool pop_if(
        std::queue<T>::value_type &res,
        std::function<bool(const typename std::queue<T>::value_type &)> cond
    ) {
        std::unique_lock<std::mutex> lock(mutex);
        if (q.empty()) return false;
        if (!cond(q.front())) return false;
        res = std::move(q.front());
        q.pop();
        return true;
    }
};
