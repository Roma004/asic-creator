#pragma once
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/register-request-manager.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/handler-device.hpp>
#include <list>
#include <mutex>

/**
 * @brief Реализация исполняющего устрйоства по-умолчанию
 *
 * Определяет поведение исполняющего устройства, работающего с простыми и
 * не требующими синхронизации по порядку следования инструкциями.
 *
 * Принцип работы:
 * - Переданная инструкция добавляется во внутренний список инструкций
 * - Производится запрос на владение ресурсами в менеджер запросов
 * - На каждой итерации handle():
 *   - Находится любая иструкция из списка, готовая к исполнению (та, которая
 *     владеет всеми необходимыми ей ресурсами)
 *   - Исполняется
 *   - Ресурсы исполненной инструкции освобождаются
 **/
class AsynkExecutorDevice : public ExecutorDeviceInterface {
  public:
    AsynkExecutorDevice(
        const InstructionRegistry &registry,
        RegisterRequestsManager &req_manager,
        std::mutex &m
    );

    void add_instruction(BaseInstruction i) override;

    bool has_instructions() const noexcept override;

    void handle() override;

  private:
    std::list<GenericInstruction> in_process;
    const InstructionRegistry &registry;
    RegisterRequestsManager &req_manager;
    std::mutex &m;
};

