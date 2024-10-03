#pragma once

#include "asic-engine/cpu/abstracts/control-device.hpp"
#include "asic-engine/cpu/registries/instruction-registry.hpp"
#include "asic-engine/cpu/register-request-manager.hpp"
#include <asic-engine/component-registry.hpp>
#include <asic-engine/cpu/abstracts/data-master-port.hpp>
#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/abstracts/instruction-form.hpp>
#include <asic-engine/cpu/abstracts/instruction-functor.hpp>
#include <asic-engine/cpu/abstracts/registers.hpp>
#include <memory>

using ExecutorDeviceRegistry = ComponentRegistry<ExecutorDeviceInterface>;
using DataMasterPortRegistry = ComponentRegistry<DataMasterPortInterface>;
using RegisterBlockRegistry = ComponentRegistry<RegisterBlock>;
using InstructionFormRegistry = ComponentRegistry<InstructionForm>;
using InstructionFunctorRegistry = ComponentRegistry<InstructionFunctor>;

/**
 * @brief Структура задаёт набор имеющихся компонент процессора
 * */
struct CPUContext {
    /** @brief указанное в проекте название процессора */
    const std::string &cpu_name;

    /** @brief зарегистрированные исполняющие устройства */
    ExecutorDeviceRegistry executors;

    /** @brief зарегистрированные инструкции */
    InstructionRegistry instructions;

    /** @brief зарегистрированные порты выхода на шину */
    DataMasterPortRegistry ports;

    /** @brief зарегистрированные блоки регистров */
    RegisterBlockRegistry register_blocks;

    /** @brief зарегистрированные форматы инструкций */
    InstructionFormRegistry forms;

    /** @brief зарегистрированные функторы */
    InstructionFunctorRegistry functors;

    /** @brief внутренний менеджер запросов к блокам регистров */
    RegisterRequestsManager req_manager;

    /** @brief Управляющее устройство */
    std::unique_ptr<ControlDeviceInterface> ctrl;
};
