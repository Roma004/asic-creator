#pragma once
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stddef.h>
#include <type_traits>

/**
 * @brief структура данных "кольцо", он же "циклический список"
 * @tparam[T] тип хранимых элементов
 * */
template <class T> class Ring {
  public:
    /** @brief тип хранимого элемента */
    using value_type = T;

    /**
     * @brief Шаблонный класс итератора кольца
     * @tparam[is_const] константность итератора
     * @note соответствует std::forward_iterator_tag
     * */
    template <bool is_const> class RingIterator;

    /** @brief Неконстнантный итератор */
    using iterator = RingIterator<false>;

    /** @brief Констнантный итератор */
    using const_iterator = RingIterator<true>;

    Ring() noexcept;
    Ring(const Ring &r);
    Ring(Ring &&r) noexcept;
    ~Ring();

    /**
     * @brief конструктор создаёт кольцо из содержимого [first, last)
     * @param[first] начало последовательности для копирования
     * @param[last] конец последовательности для копирования
     * */
    template <class InputIt> Ring(InputIt first, InputIt last);

    /**
     * @brief
     *   построить кольцо, скопировав элементы из указанного списка
     *   инициализации
     * @param[l] сисок инициализации с элементами
     * */
    Ring(std::initializer_list<value_type> l);

    Ring &operator=(const Ring &r) noexcept;
    Ring &operator=(Ring &&) noexcept;

    /**
     * @brief извлечь элемент из кольца
     * @note вызов pop для пустого кольца не возымеет никакого эффекта
     * */
    void pop() noexcept;

    /**
     * @brief положить элемент в кольцо
     * @param[item] элемент для помещения в кольцо
     * */
    void push(const value_type &item);
    void push(value_type &&item);

    /** @brief очистить кольцо */
    void clear() noexcept;

    /**
     * @brief обменять кольца содержимым
     * @param[o] кольцо для обмена содержимым
     * */
    void swap(Ring &o) noexcept;

    /**
     * @brief Первый в очереди на извлечение из кольца элемент
     * @returns ссылка на первый элемент кольца
     * */
    value_type &front();
    const value_type &front() const;

    /**
     * @brief Последний в очереди на извлечение из кольца элемент
     * @returns ссылка на последний элемент кольца
     * */
    value_type &back();
    const value_type &back() const;

    /**
     * @brief Создать элемент кольца с заданными параметрами
     * @note Созданный элемент размещается последним в кольце
     * @tparam[Args] перечисление типов аргументов конструктора элемента кольца
     * @param[args] аргументы конструктора элемента кольца
     * */
    template <typename... Args>
        requires(std::constructible_from<T, Args...>)
    void emplace(Args &&...args);

    /**
     * @brief Является ли кольцо пустым
     * @returns
     *   true, если нет ни дного элемента в кольце
     *   false в противном случае
     * */
    constexpr bool empty() const noexcept;

    /** @brief получить количество элементов в кольце */
    constexpr size_t size() const noexcept;

    /** @brief получить итератор головы кольца */
    iterator begin() noexcept;
    const_iterator begin() const noexcept;

    /** @brief получить итератор окончания кольца */
    iterator end() noexcept;
    const_iterator end() const noexcept;

    /** @brief получить итератор головы кольца */
    const_iterator cbegin() const noexcept;

    /** @brief получить итератор окончания кольца */
    const_iterator cend() const noexcept;

  private:
    friend iterator;
    friend const_iterator;

    struct Node {
        value_type val;
        Node *next;
    };

    /**
     * @brief
     *   создать новый узел, значение будет построено с заданным набором
     *   параметров
     * @param[args] аргументы для построения значения узла
     * @returns Указатель на созданный узел
     * */
    template <typename... Args> Node *allocate(Args... args);

    /** @brief разрушить узел и освободить из под него память */
    void deallocate(Node *n) noexcept;

    /** @brief добавить узел в кольцо */
    void push_node(Node *n);

    /** @brief удалить узел из кольца (и разрушить его) */
    void pop_node() noexcept;

    Node *tail;
    size_t n_nodes;
};

template <class T> template <bool is_const> class Ring<T>::RingIterator {
  public:
    using value_type = Ring::value_type;
    using difference_type = ptrdiff_t;
    using pointer =
        std::conditional_t<is_const, const value_type, value_type> *;
    using reference =
        std::conditional_t<is_const, const value_type, value_type> &;
    using iterator_category = std::forward_iterator_tag;

    RingIterator() noexcept;

    template <bool other_const>
    RingIterator(const RingIterator<other_const> &) noexcept
        requires(is_const >= other_const);

    template <bool other_const>
    RingIterator &operator=(const RingIterator<other_const> &) noexcept
        requires(is_const >= other_const);

    reference operator*() const noexcept;
    pointer operator->() const noexcept;

    template <bool other_const>
    bool operator==(const RingIterator<other_const> &) const noexcept;

    RingIterator &operator++() noexcept;
    RingIterator operator++(int) noexcept;

  private:
    const Ring *r;
    Node *node;

    RingIterator(const Ring &r, Node *node) noexcept;

    friend Ring;
    friend RingIterator<!is_const>;
};

static_assert(std::forward_iterator<Ring<int>::iterator>);
static_assert(std::forward_iterator<Ring<int>::const_iterator>);

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const>::RingIterator() noexcept :
    r(nullptr), node(nullptr) {}

template <class T>
template <bool is_const>
template <bool other_const>
Ring<T>::RingIterator<is_const>::RingIterator(const RingIterator<other_const> &o
) noexcept
    requires(is_const >= other_const)
    : r(o.r), node(o.node) {}

template <class T>
template <bool is_const>
template <bool other_const>
Ring<T>::RingIterator<is_const> &
Ring<T>::RingIterator<is_const>::operator=(const RingIterator<other_const> &o
) noexcept
    requires(is_const >= other_const)
{
    r = o.r;
    node = o.node;
    return *this;
}

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const>::reference
Ring<T>::RingIterator<is_const>::operator*() const noexcept {
    return node->val;
}

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const>::pointer
Ring<T>::RingIterator<is_const>::operator->() const noexcept {
    return &node->val;
}

template <class T>
template <bool is_const>
template <bool other_const>
bool Ring<T>::RingIterator<is_const>::operator==(
    const RingIterator<other_const> &o
) const noexcept {
    return node == o.node;
}

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const> &
Ring<T>::RingIterator<is_const>::operator++() noexcept {
    if (node == r->tail) node = nullptr;
    else node = node->next;
    return *this;
}

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const> Ring<T>::RingIterator<is_const>::operator++(int
) noexcept {
    RingIterator res(*this);
    ++(*this);
    return res;
}

template <class T>
template <bool is_const>
Ring<T>::RingIterator<is_const>::RingIterator(
    const Ring &r, Node *node
) noexcept : r(&r), node(node) {}

template <class T> Ring<T>::Ring() noexcept : tail(nullptr), n_nodes(0) {}

template <class T> Ring<T>::Ring(const Ring &r) : Ring(r.begin(), r.end()) {}

template <class T> Ring<T>::Ring(Ring &&o) noexcept : Ring() { swap(o); }

template <class T>
template <class InputIt>
Ring<T>::Ring(InputIt first, InputIt last) : Ring() {
    try {
        for (; first != last; ++first) { push(*first); }
    } catch (...) {
        clear();
        throw;
    }
}

template <class T>
Ring<T>::Ring(std::initializer_list<value_type> l) : Ring(l.begin(), l.end()) {}

template <class T> Ring<T> &Ring<T>::operator=(const Ring &r) noexcept {
    Ring res(r);
    swap(res);
    return *this;
}

template <class T> Ring<T> &Ring<T>::operator=(Ring &&o) noexcept {
    swap(o);
    return *this;
}

template <class T> Ring<T>::~Ring() { clear(); }

template <class T> void Ring<T>::pop() noexcept { pop_node(); }

template <class T> void Ring<T>::push(const value_type &item) {
    push_node(allocate(item));
}

template <class T> void Ring<T>::push(value_type &&item) {
    push_node(allocate(std::move(item)));
}

template <class T> void Ring<T>::clear() noexcept {
    while (!empty()) pop();
}

template <class T> void Ring<T>::swap(Ring &o) noexcept {
    std::swap(tail, o.tail);
    std::swap(n_nodes, o.n_nodes);
}

template <class T> Ring<T>::value_type &Ring<T>::front() {
    return tail->next->val;
}

template <class T> const Ring<T>::value_type &Ring<T>::front() const {
    return tail->next->val;
}

template <class T> Ring<T>::value_type &Ring<T>::back() { return tail->val; }

template <class T> const Ring<T>::value_type &Ring<T>::back() const {
    return tail->val;
}

template <class T>
template <typename... Args>
    requires(std::constructible_from<T, Args...>)
void Ring<T>::emplace(Args &&...args) {
    push_node(allocate(std::forward<Args>(args)...));
}

template <class T> constexpr bool Ring<T>::empty() const noexcept {
    return n_nodes == 0;
}

template <class T> constexpr size_t Ring<T>::size() const noexcept {
    return n_nodes;
}

template <class T> Ring<T>::iterator Ring<T>::begin() noexcept {
    return iterator(*this, tail->next);
}

template <class T> Ring<T>::iterator Ring<T>::end() noexcept {
    return iterator(*this, nullptr);
}

template <class T> Ring<T>::const_iterator Ring<T>::begin() const noexcept {
    return cbegin();
}

template <class T> Ring<T>::const_iterator Ring<T>::end() const noexcept {
    return cend();
}

template <class T> Ring<T>::const_iterator Ring<T>::cbegin() const noexcept {
    return const_iterator(*this, tail->next);
}

template <class T> Ring<T>::const_iterator Ring<T>::cend() const noexcept {
    return const_iterator(*this, nullptr);
}

template <class T>
template <typename... Args>
Ring<T>::Node *Ring<T>::allocate(Args... args) {
    return new Node{
        .val = value_type(std::forward<Args>(args)...), .next = nullptr
    };
}

template <class T> void Ring<T>::deallocate(Node *n) noexcept { delete n; }

template <class T> void Ring<T>::push_node(Node *n) {
    if (tail) {
        n->next = tail->next;
        tail->next = n;
    } else n->next = n;
    tail = n;
    n_nodes += 1;
}

template <class T> void Ring<T>::pop_node() noexcept {
    if (!tail) return;
    Node *n;
    if (tail->next == tail) {
        n = tail;
        tail = nullptr;
    } else {
        n = tail->next;
        tail->next = n->next;
    }
    n_nodes -= 1;
    deallocate(n);
}
