#pragma once
#include <algorithm>
#include <cstring>
#include <stdexcept>

class TestCompositeType {
  public:
    TestCompositeType() : dyn_data(nullptr), msize(0) {}

    TestCompositeType(const std::string &name, size_t size) :
        name(name), dyn_data(new int[size]{}), msize(size) {}

    TestCompositeType(
        const std::string &name, std::initializer_list<int> content
    ) : TestCompositeType(name, content.size()) {
        std::ranges::copy(content, dyn_data);
    }

    TestCompositeType(const TestCompositeType &o) :
        TestCompositeType(o.name, o.msize) {
        std::copy(o.dyn_data, o.dyn_data + o.msize, dyn_data);
    }

    TestCompositeType &operator=(const TestCompositeType &o) {
        TestCompositeType res(o);
        swap(res);
        return *this;
    }

    ~TestCompositeType() {
        if (dyn_data) delete[] dyn_data;
    }

    void swap(TestCompositeType &o) {
        std::swap(name, o.name);
        std::swap(msize, o.msize);
        std::swap(dyn_data, o.dyn_data);
    }

    bool operator==(const TestCompositeType &o) const noexcept {
        return name == o.name && msize == o.msize
            && std::memcmp(dyn_data, o.dyn_data, msize) == 0;
    }

    size_t size() const noexcept { return msize; }

  private:
    std::string name;
    int *dyn_data;
    size_t msize;
};

class UnstableConstructable {
  public:
    UnstableConstructable(const std::string &name) : val(name, {1, 2, 3, 4}) {
        if (name == "fail") throw std::runtime_error("F*CK YOU ALL");
    }

    bool operator==(const UnstableConstructable &o) const noexcept {
        return val == o.val;
    }

  private:
    TestCompositeType val;
};

class UnstableCopyable {
  public:
    UnstableCopyable(const std::string &name) :
        name(name), val(name, {1, 2, 3, 4}) {}

    UnstableCopyable(const UnstableCopyable &o) : val(o.val), name(o.name) {
        if (name == "fail") throw std::runtime_error("F*CK YOU ALL");
    }

    UnstableCopyable(UnstableCopyable &&o) noexcept :
        val(o.val), name(o.name) {}

    UnstableCopyable &operator=(const UnstableCopyable &o) {
        UnstableCopyable res(o);
        swap(res);
        return *this;
    }

    UnstableCopyable &operator=(UnstableCopyable &&o) noexcept {
        swap(o);
        return *this;
    }

    void swap(UnstableCopyable &o) noexcept {
        name.swap(o.name);
        val.swap(o.val);
    }

    bool operator==(const UnstableCopyable &o) const noexcept {
        return val == o.val;
    }

  private:
    std::string name;
    TestCompositeType val;
};

class UnstableMovable {
  public:
    UnstableMovable(const std::string &name) :
        name(name), val(name, {1, 2, 3, 4}) {}

    UnstableMovable(UnstableMovable &&o) :
        val(std::move(o.val)), name(std::move(o.name)) {
        if (name == "fail") throw std::runtime_error("OOPS!! Moved!");
    }

    UnstableMovable &operator=(UnstableMovable &&o) {
        if (name == "fail") throw std::runtime_error("OOPS!! Moved!");
        swap(o);
        return *this;
    }

    void swap(UnstableMovable &o) noexcept {
        name.swap(o.name);
        val.swap(o.val);
    }

    bool operator==(const UnstableMovable &o) const noexcept {
        return val == o.val;
    }

  private:
    std::string name;
    TestCompositeType val;
};

template <class T> std::initializer_list<T> &get_init_list();

template <> inline std::initializer_list<int> &get_init_list<int>() {
    static auto res = {1, 2, 3, 4, 5};
    return res;
}

template <>
inline std::initializer_list<TestCompositeType> &
get_init_list<TestCompositeType>() {
    static auto res = {
        TestCompositeType("asd", {1, 2, 3, 8}),
        TestCompositeType("sdf", {2, 3, 1, 5, 7}),
        TestCompositeType("dfg", {1, 2, 3, 8, 9})
    };
    return res;
}

