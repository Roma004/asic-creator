#pragma once
#include <functional>
#include <mutex>
#include <optional>
#include <ring.hpp>

template <std::movable T> class ConcurentQueue {
  public:
    using value_type = T;

    /** @brief конструктор по-умолчанию */
    ConcurentQueue() noexcept;

    /**
     * @brief копирующий конструктор
     * @note копирование очереди приведёт к созданию нового мьютекса
     * */
    ConcurentQueue(const ConcurentQueue<T> &);

    /**
     * @brief перемещающий конструктор
     * @note перемещение очереди приведёт к созданию нового мьютекса
     * */
    ConcurentQueue(ConcurentQueue<T> &&) noexcept;

    /**
     * @brief копирующий опреатор присваивания
     * @note
     *   копирующее присваиваивание очереди приведёт к копированию только
     *   содержимого очереди. Мьютекс, созданный при инициализации объекта
     *   останется на месте
     * */
    ConcurentQueue &operator=(const ConcurentQueue<T> &o);

    /**
     * @brief перемещающий опреатор присваивания
     * @note
     *   перемещающее присваиваивание очереди приведёт к перемещению только
     *   содержимого очереди. Мьютексы, созданные при инициализации обоих
     *   объектов останутся на месте
     * */
    ConcurentQueue &operator=(ConcurentQueue<T> &&o) noexcept;

    /**
     * @brief конструктор создаёт очередь из содержимого [first, last)
     * @param[first] начало последовательности для копирования
     * @param[last] конец последовательности для копирования
     * */
    template <class InputIt> ConcurentQueue(InputIt first, InputIt last);

    /**
     * @brief
     *   построить очередь, скопировав элементы из указанного списка
     *   инициализации
     * @param[l] сисок инициализации с элементами
     * */
    ConcurentQueue(std::initializer_list<value_type> l);

    /**
     * @brief извлечь элемент из очереди
     * @returns std::future значения головы очереди, которое было извлечено
     * */
    std::optional<value_type> pop() noexcept;

    /**
     * @brief извлечь элемент из головы очереди, если он удовлетворяет условию
     * */
    std::optional<value_type>
        pop_if(std::function<bool(const value_type &)>) noexcept;

    /**
     * @brief положить элемент в очередь
     * @param[item] элемент для помещения в кольцо
     * */
    void push(const value_type &item);
    void push(value_type &&item);

    /** @brief очистить очередь */
    void clear() noexcept;

    /**
     * @brief обменять очереди содержимым
     * @param[o] очередь для обмена содержимым
     * */
    void swap(ConcurentQueue &o) noexcept;

    /**
     * @brief Первый в очереди элемент
     * @returns ссылка на значение первого элемента
     * */
    value_type &front();
    const value_type &front() const;

    /**
     * @brief Последний в очереди элемент
     * @returns ссылка на значение последнего элемента
     * */
    value_type &back();
    const value_type &back() const;

    /**
     * @brief Создать элемент очереди с заданными параметрами
     * @note Созданный элемент размещается последним в очереди
     * @tparam[Args] перечисление типов аргументов конструктора элемента очереди
     * @param[args] аргументы конструктора элемента очереди
     * */
    template <typename... Args>
        requires(std::constructible_from<T, Args...>)
    void emplace(Args &&...args);

    constexpr size_t size() const noexcept { return ring.size(); }
    constexpr bool empty() const noexcept { return ring.empty(); }

  private:
    Ring<T> ring;
    mutable std::mutex m;
};

template <std::movable T>
ConcurentQueue<T>::ConcurentQueue() noexcept : ring(), m() {}

template <std::movable T>
ConcurentQueue<T>::ConcurentQueue(const ConcurentQueue<T> &o) :
    ring(o.ring), m() {}

template <std::movable T>
ConcurentQueue<T>::ConcurentQueue(ConcurentQueue<T> &&o) noexcept :
    ring(std::move(o.ring)), m(){};

template <std::movable T>
template <class InputIt>
ConcurentQueue<T>::ConcurentQueue(InputIt first, InputIt last) :
    ring(first, last), m() {}

template <std::movable T>
ConcurentQueue<T>::ConcurentQueue(std::initializer_list<value_type> l) :
    ring(l), m() {}

template <std::movable T>
ConcurentQueue<T> &ConcurentQueue<T>::operator=(const ConcurentQueue<T> &o) {
    ConcurentQueue q(o);
    swap(q);
    return *this;
}

template <std::movable T>
ConcurentQueue<T> &ConcurentQueue<T>::operator=(ConcurentQueue<T> &&o) noexcept {
    ConcurentQueue q(o);
    swap(q);
    return *this;
}

template <std::movable T> std::optional<T> ConcurentQueue<T>::pop() noexcept {
    std::unique_lock<std::mutex> l(m);
    if (ring.empty()) return std::nullopt;
    std::optional<value_type> res(std::move(ring.front()));
    ring.pop();
    return res;
}

template <std::movable T>
std::optional<T>
ConcurentQueue<T>::pop_if(std::function<bool(const value_type &)> cond
) noexcept {
    std::unique_lock<std::mutex> l(m);
    if (ring.empty()) return std::nullopt;
    if (!cond(ring.front())) return std::nullopt;
    std::optional<value_type> res(std::move(ring.front()));
    ring.pop();
    return res;
}

template <std::movable T> void ConcurentQueue<T>::push(const value_type &item) {
    std::unique_lock<std::mutex> l(m);
    ring.push(item);
}
template <std::movable T> void ConcurentQueue<T>::push(value_type &&item) {
    std::unique_lock<std::mutex> l(m);
    ring.push(std::move(item));
}

template <std::movable T> void ConcurentQueue<T>::clear() noexcept {
    std::unique_lock<std::mutex> l(m);
    ring.clear();
}

template <std::movable T>
void ConcurentQueue<T>::swap(ConcurentQueue<T> &o) noexcept {
    std::unique_lock<std::mutex> l1(m);
    std::unique_lock<std::mutex> l2(o.m);
    ring.swap(o.ring);
}

template <std::movable T>
ConcurentQueue<T>::value_type &ConcurentQueue<T>::front() {
    std::unique_lock<std::mutex> l(m);
    return ring.front();
}

template <std::movable T>
const ConcurentQueue<T>::value_type &ConcurentQueue<T>::front() const {
    std::unique_lock<std::mutex> l(m);
    return ring.front();
}

template <std::movable T>
ConcurentQueue<T>::value_type &ConcurentQueue<T>::back() {
    std::unique_lock<std::mutex> l(m);
    return ring.back();
}

template <std::movable T>
const ConcurentQueue<T>::value_type &ConcurentQueue<T>::back() const {
    std::unique_lock<std::mutex> l(m);
    return ring.back();
}

template <std::movable T>
template <typename... Args>
    requires(std::constructible_from<T, Args...>)
void ConcurentQueue<T>::emplace(Args &&...args) {
    std::unique_lock<std::mutex> l(m);
    ring.emplace(std::forward<Args>(args)...);
}
