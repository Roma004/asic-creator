#pragma once


#include "asic-engine/component-registry.hpp"
#include "asic-engine/data-master/abstracts/endpoints.hpp"

using BrigdeRegistry = ComponentRegistry<BridgeInterface>;
using DefaultDeviceRegisry = ComponentRegistry<DefaultDeviceInterface>;
using EndpointRegistry = ComponentRegistry<EndpointInterface>;
using MasterGateRegistry = ComponentRegistry<RootInterface>;
using SwitchRegistry = ComponentRegistry<SwitchInterface>;

/**
 * @brief Структура задаёт набор имеющихся компонент шины
 * */
struct BusContext {
    /** @brief указанное в проекте название шины */
    // const std::string &bus_name;

    BrigdeRegistry bridges;
    DefaultDeviceRegisry default_devs;
    EndpointRegistry endpoints;
    MasterGateRegistry masters;
    SwitchRegistry switches;
};
