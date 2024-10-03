#include <asic-engine/asic/configs.hpp>
#include <asic-engine/asic/util.hpp>


BaseConfig::BaseConfig(const nlohmann::json &j) :
    id(json_get_obj(j, "id")),
    settings(settings_from_json(json_get_obj(j, "settings"))) {}

DataMasterPortConfig::DataMasterPortConfig(const nlohmann::json &j) :
    BaseConfig(j),
    bus_master_name(json_get_obj(j, "bus-gate")) {}

InstructionConfig::InstructionConfig(const nlohmann::json &j) :
    opcode(json_get_obj(j, "opcode")), form_name(json_get_obj(j, "form")),
    functor_name(json_get_obj(j, "functor")),
    executor_name(json_get_obj(j, "executor")) {}

BusMasterConfig::BusMasterConfig(const nlohmann::json &j) :
    BaseConfig(j), slave_name(json_get_obj(j, "slave")) {}

BusSwitchSlaveConfig::BusSwitchSlaveConfig(const nlohmann::json &j) :
    base(json_get_obj(j, "base")), size(json_get_obj(j, "size")),
    slave_name(json_get_obj(j, "slave")) {}

BusSwitchConfig::BusSwitchConfig(const nlohmann::json &j) :
    BaseConfig(j), default_slave_name(json_get_obj(j, "default-slave")),
    slaves(json_get_obj(j, "slaves").begin(), json_get_obj(j, "slaves").end()) {
}

BusBridgeConfig::BusBridgeConfig(const nlohmann::json &j) :
    BaseConfig(j), slave_name(json_get_obj(j, "slave")) {}

PeripheralConfig::PeripheralConfig(const nlohmann::json &j) :
    BaseConfig(j), bus_endpoint_name(json_get_obj(j, "gate")) {}

CPUConfig::CPUConfig(const nlohmann::json &j) :
    ctrl(json_get_obj(j, "control")), ports(json_get_obj(j, "ports")),
    executors(json_get_obj(j, "executors")),
    registers(json_get_obj(j, "registers")), forms(json_get_obj(j, "forms")),
    functors(json_get_obj(j, "functors")),
    instructions(
        json_get_obj(j, "instructions").begin(),
        json_get_obj(j, "instructions").end()
    ) {}

BusConfig::BusConfig(const nlohmann::json &j) :
    masters(json_get_obj(j, "masters")), switches(json_get_obj(j, "switches")),
    bridges(json_get_obj(j, "bridges")),
    default_devs(json_get_obj(j, "default-devices")),
    endpoints(json_get_obj(j, "endpoints")) {}

ASICConfig::ASICConfig(const nlohmann::json &j) :
    cpus(json_get_obj(j, "cpus")), bus(json_get_obj(j, "bus")),
    peripherals(json_get_obj(j, "peripherals")) {}
