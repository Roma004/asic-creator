#pragma once
#include "tristate.hpp"
#include <cstddef>
#include <cstdint>
#include <istream>
#include <iterator>

#define CEIL_DIV(a, b) ((a + b - 1) / b);

/**
 * @brief
 *     class incapsulates memory management for storing Tristates in most
 *     optimal way
 */
class TriRegisterContainer {
  private:
    using st = Tristate::STATE;
    struct RegChunk;

  public:
    /**
     * @brief
     *     class defines object, acts as a reference to the element of container
     */
    struct reference;

    /**
     * @brief
     *     class defines object, acts as a constant reference to the element of
     *     container
     */
    struct const_reference;

    /** @brief basic class of iterator */
    template <class RefT> class basic_iterator;
    /** @brief basic class of forwards iterator (const or not) */
    template <class RefT> class forward_iterator;
    /** @brief basic class of backwards iterator (const or not) */
    template <class RefT> class backward_iterator;

    /** @brief forward iterator */
    using iterator = forward_iterator<reference>;

    /** @brief reverse iterator */
    using reverse_iterator = backward_iterator<reference>;

    /** @brief constant forward iterator */
    using const_iterator = forward_iterator<const_reference>;

    /** @brief constant reverse iterator */
    using const_reverse_iterator = backward_iterator<const_reference>;

    /** @brief sentinal (the end of iteration), comparable with any iterator */
    class sentinel;

  private:
    RegChunk *chunks;
    size_t nchunks;
    size_t nsigns;

    void set_chunk(size_t ch_idx, Tristate st) noexcept;
    void set(int idx, const Tristate &state) noexcept;

    void resize_unsafe(size_t new_size);

    constexpr const reference get_ref(int idx) const noexcept;

  public:
    /** @brief constuct an empty container by default */
    TriRegisterContainer();

    /**
     * @brief constuct container of @par nsigns elements with value def_st
     */
    TriRegisterContainer(size_t nsigns, Tristate def_st = st::X);

    /** @brief copy constuctor */
    TriRegisterContainer(const TriRegisterContainer &c);

    /** @brief move constuctor */
    TriRegisterContainer(TriRegisterContainer &&c);

    /**
     * @brief constuct container from any sequence between from and to
     */
    template <class Iter>
    TriRegisterContainer(Iter from, Iter to) : TriRegisterContainer() {
        int cur_size = 16;
        resize_unsafe(cur_size / 4);
        std::for_each(from, to, [&](auto state) {
            if (nsigns == cur_size) {
                cur_size += 16;
                resize_unsafe(cur_size / 4);
            }
            set(nsigns++, state);
            nchunks = CEIL_DIV(nsigns, 4);
        });
        resize_unsafe(nchunks);
    }
    /** @brief destructor */
    ~TriRegisterContainer();

    /**
     * @brief operator for getting modifiable reference to the signal
     * @returns reference object with ability to assign Tristate and
     * derefer to get Tristate
     */
    const reference operator[](int idx) const;

    /** @brief copy assignment operator */
    TriRegisterContainer &operator=(const TriRegisterContainer &ct) noexcept;

    /** @brief move assignment operator */
    TriRegisterContainer &operator=(TriRegisterContainer &&ct) noexcept;

    /** @brief get the number of signals stored in container */
    constexpr size_t size() const noexcept { return nsigns; }

    /** @brief iterator to the begin of signals sequence (left->right) */
    iterator begin() noexcept;
    /** @brief iterator to the end of signals sequence (left->right) */
    iterator end() noexcept;

    /** @brief constant iterator to the begin of signals sequence (left->right)
     */
    const_iterator begin() const noexcept;
    /** @brief constant iterator to the end of signals sequence (left->right) */
    const_iterator end() const noexcept;

    /** @brief constant iterator to the begin of signals sequence (left->right)
     */
    const_iterator cbegin() const noexcept;
    /** @brief constant iterator to the end of signals sequence (left->right) */
    const_iterator cend() const noexcept;

    /** @brief iterator to the begin of signals sequence (right->left) */
    reverse_iterator rbegin() noexcept;
    /** @brief iterator to the end of signals sequence (right->left) */
    reverse_iterator rend() noexcept;

    /** @brief constant iterator to the begin of signals sequence (right->left)
     */
    const_reverse_iterator rbegin() const noexcept;
    /** @brief constant iterator to the end of signals sequence (right->left) */
    const_reverse_iterator rend() const noexcept;

    /** @brief constant iterator to the begin of signals sequence (right->left)
     */
    const_reverse_iterator crbegin() const noexcept;
    /** @brief constant iterator to the end of signals sequence (right->left) */
    const_reverse_iterator crend() const noexcept;
};

struct TriRegisterContainer::RegChunk {
    uint8_t data;

    constexpr static inline uint8_t st2val(st state) noexcept;
    constexpr static inline st val2st(uint8_t val) noexcept;
    Tristate get(int idx) const noexcept;
    void set(int idx, Tristate state) noexcept;
    void fill(Tristate state) noexcept;
};

struct TriRegisterContainer::reference {
    RegChunk *chunks;
    int idx;

    /** @brief transform reference to Tristate signal */
    operator Tristate() const noexcept;
    /** @brief transform reference to Tristate::STATE */
    operator st() const noexcept;
    /** @brief assignment operator */
    const reference &operator=(const Tristate &state) const noexcept;
    /** @brief comparison with other reference operator */
    bool operator==(const reference &o) const noexcept;
    /** @brief comparison with other reference operator */
    bool operator!=(const reference &o) const noexcept;
    /** @brief get referent-type from std::istream */
    friend std::istream &operator>>(std::istream &in, reference &ref) {
        Tristate val;
        in >> val;
        ref = val;
        return in;
    }
};

struct TriRegisterContainer::const_reference {
    const RegChunk *chunks;
    int idx;

    /** @brief constructor from non-constant reference */
    const_reference(const reference &ref);

    /** @brief transform reference to Tristate signal */
    operator Tristate() const noexcept;
    /** @brief transform reference to Tristate::STATE */
    operator st() const noexcept;
    const const_reference &operator=(const Tristate &state) noexcept = delete;
    /** @brief comparison with other reference operator */
    bool operator==(const const_reference &o) const noexcept;
    /** @brief comparison with other reference operator */
    bool operator!=(const const_reference &o) const noexcept;
};

template <class RefT> class TriRegisterContainer::basic_iterator {
  protected:
    RefT ref;
    size_t nsigns;

    constexpr void inc(int n) noexcept {
        if (ref.idx == nsigns) return;
        ref.idx += 1;
    }
    constexpr void dec(int n) noexcept {
        if (ref.idx == -1) return;
        ref.idx -= 1;
    }
    constexpr bool eq(const basic_iterator &o) const noexcept {
        return ref.chunks == o.ref.chunks && ref.idx == o.ref.idx;
    }

  public:
    using value_type = Tristate;
    using difference_type = std::ptrdiff_t;
    /** @brief default constuctor */
    basic_iterator() {}
    /** @brief constructor from reference-type and max number of signals */
    basic_iterator(const RefT &ref, size_t nsigns) : ref(ref), nsigns(nsigns) {}
    /** @brief copy constructor */
    basic_iterator(const basic_iterator &it) : ref(it.ref), nsigns(it.nsigns) {}
    /** @brief iterator dereference operator */
    const RefT &operator*() const noexcept { return ref; }
};

template <class RefT>
class TriRegisterContainer::forward_iterator : public basic_iterator<RefT> {
  protected:
    using basic_iterator<RefT>::ref;
    using basic_iterator<RefT>::nsigns;
    using basic_iterator<RefT>::inc;
    using basic_iterator<RefT>::dec;
    using basic_iterator<RefT>::eq;

  public:
    friend class sentinel;
    using basic_iterator<RefT>::basic_iterator;
    using typename basic_iterator<RefT>::value_type;
    using typename basic_iterator<RefT>::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;
    using reference = RefT;
    /** @brief increment iterator to the left->right direction */
    forward_iterator &operator++() noexcept {
        inc(1);
        return *this;
    }
    /** @brief increment iterator forwards to @par idx steps*/
    forward_iterator &operator+(int idx) noexcept {
        inc(idx);
        return *this;
    }
    /** @brief comparison with iterator operator */
    bool operator==(const forward_iterator &o) const noexcept { return eq(o); }
    /** @brief comparison with iterator operator */
    bool operator!=(const forward_iterator &o) const noexcept { return !eq(o); }
    /** @brief comparison with sentinel operator */
    bool operator==(const sentinel &) const noexcept {
        return ref.idx == nsigns;
    }
    /** @brief comparison with sentinel operator */
    bool operator!=(const sentinel &) const noexcept {
        return ref.idx != nsigns;
    }
    /** @brief increment iterator to the left->right direction */
    void operator++(int) { ++*this; }
    /** @brief conversion to the const reverse iterator */
    operator const_iterator() const noexcept {
        return const_iterator(const_reference(ref), nsigns);
    }
};

template <class RefT>
class TriRegisterContainer::backward_iterator : public basic_iterator<RefT> {
  protected:
    using basic_iterator<RefT>::ref;
    using basic_iterator<RefT>::nsigns;
    using basic_iterator<RefT>::inc;
    using basic_iterator<RefT>::dec;
    using basic_iterator<RefT>::eq;

  public:
    friend class sentinel;
    using basic_iterator<RefT>::basic_iterator;
    using typename basic_iterator<RefT>::value_type;
    using typename basic_iterator<RefT>::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;
    using reference = RefT;

    /** @brief increment iterator to the right->left direction */
    backward_iterator &operator++() noexcept {
        dec(1);
        return *this;
    }
    /** @brief increment iterator backwards to @par idx steps */
    backward_iterator &operator+(int idx) noexcept {
        dec(idx);
        return *this;
    }
    /** @brief comparison with reverse iterator operator */
    bool operator==(const backward_iterator &o) const noexcept { return eq(o); }
    /** @brief comparison with reverse iterator operator */
    bool operator!=(const backward_iterator &o) const noexcept {
        return !eq(o);
    }
    /** @brief comparison with sentinel operator */
    bool operator==(const sentinel &) const noexcept { return ref.idx == 0; }
    /** @brief comparison with sentinel operator */
    bool operator!=(const sentinel &) const noexcept { return ref.idx != 0; }
    /** @brief increment iterator to the right->left direction */
    void operator++(int) { ++*this; }

    /** @brief conversion to the const reverse iterator */
    operator const_reverse_iterator() const noexcept {
        return const_reverse_iterator(const_reference(ref), nsigns);
    }
};

class TriRegisterContainer::sentinel {
  public:
    /** @brief copy assignment operator */
    sentinel &operator=(const sentinel &it) { return *this; }

    /** @brief comparison with iterator operator */
    bool operator==(const iterator &it) const noexcept { return it == *this; }

    /** @brief comparison with iterator operator */
    bool operator!=(const iterator &it) const noexcept { return it != *this; }

    /** @brief comparison with reverse_iterator operator */
    bool operator==(const reverse_iterator &it) const noexcept {
        return it == *this;
    }

    /** @brief comparison with reverse_iterator operator */
    bool operator!=(const reverse_iterator &it) const noexcept {
        return it != *this;
    }
};
