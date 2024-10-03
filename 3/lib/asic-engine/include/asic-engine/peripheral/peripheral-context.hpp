#pragma once
#include "asic-engine/component-registry.hpp"
#include "asic-engine/peripheral/abstracts/peripheral.hpp"

using PeripheralRegistry = ComponentRegistry<PeripheralInterface>;

struct PeripheralContext {
    PeripheralRegistry perhps;
};
