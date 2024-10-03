#pragma once
#include "util.hpp"

struct BaseConfig {
    std::string id;
    ModuleSettings settings;

    BaseConfig(const nlohmann::json &j);
};

struct DataMasterPortConfig : BaseConfig {
    std::string bus_master_name;

    DataMasterPortConfig(const nlohmann::json &j);
};

using ControlDeviceConfig = BaseConfig;
using ExecutorsConfig = BaseConfig;
using RegistersConfig = BaseConfig;
using FormsConfig = BaseConfig;
using FunctorsConfig = BaseConfig;

struct InstructionConfig {
    uint32_t opcode;
    std::string form_name;
    std::string functor_name;
    std::string executor_name;

    InstructionConfig(const nlohmann::json &j);
};

struct BusMasterConfig : BaseConfig {
    std::string slave_name;

    BusMasterConfig(const nlohmann::json &j);
};

struct BusSwitchSlaveConfig {
    uint32_t base;
    uint32_t size;
    std::string slave_name;

    BusSwitchSlaveConfig(const nlohmann::json &j);
};

struct BusSwitchConfig : BaseConfig {
    std::vector<BusSwitchSlaveConfig> slaves;
    std::string default_slave_name;

    BusSwitchConfig(const nlohmann::json &j);
};

struct BusBridgeConfig : BaseConfig {
    std::string slave_name;

    BusBridgeConfig(const nlohmann::json &j);
};

using BusDefaultDeviceConfig = BaseConfig;
using BusEndpointConfig = BaseConfig;

struct PeripheralConfig : BaseConfig {
    std::string bus_endpoint_name;

    PeripheralConfig(const nlohmann::json &j);
};

struct CPUConfig {
    ControlDeviceConfig ctrl;
    Dict<DataMasterPortConfig> ports;
    Dict<ExecutorsConfig> executors;
    Dict<RegistersConfig> registers;
    Dict<FormsConfig> forms;
    Dict<FunctorsConfig> functors;
    std::vector<InstructionConfig> instructions;

    CPUConfig(const nlohmann::json &j);
};

struct BusConfig {
    Dict<BusMasterConfig> masters;
    Dict<BusSwitchConfig> switches;
    Dict<BusBridgeConfig> bridges;
    Dict<BusDefaultDeviceConfig> default_devs;
    Dict<BusEndpointConfig> endpoints;

    BusConfig(const nlohmann::json &j);
};

struct ASICConfig {
    Dict<CPUConfig> cpus;
    BusConfig bus;
    Dict<PeripheralConfig> peripherals;

    ASICConfig(const nlohmann::json &j);
};


