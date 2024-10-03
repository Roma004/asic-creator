#pragma once
#include <cstdint>

#define OOPS 0x53504f4f

typedef uint32_t addr_t;
typedef uint32_t ofst_t;
typedef uint32_t word_t;

enum OPType { READ, WRITE, ATOMIC };

/** @brief Задаёт диапазон адрессов через базовый адрес и размер */
struct AddrRange {
    addr_t addr;
    ofst_t offset;

    bool operator==(const AddrRange &) const = default;
    bool operator!=(const AddrRange &) const = default;
};
