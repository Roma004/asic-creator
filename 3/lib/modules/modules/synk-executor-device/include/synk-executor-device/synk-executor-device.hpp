#pragma once
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/cpu/abstracts/executor-device.hpp>
#include <asic-engine/cpu/register-request-manager.hpp>
#include <asic-engine/cpu/generic-instruction.hpp>
#include <asic-engine/cpu/registries/instruction-registry.hpp>
#include <asic-engine/handler-device.hpp>
#include <mutex>
#include <queue>

/**
 * @brief Реализация строго последовательного исполняющего устрйоства
 *
 * Определяет поведение исполняющего устройства, исполняющего инструкции в
 * порядке их передачи на исполнение.
 *
 * Принцип работы:
 * - Переданная инструкция добавляется во внутренний список инструкций
 * - Производится запрос на владение ресурсами в менеджер запросов
 * - На каждой итерации handle():
 *   - если первая в очереди инструкция готова к исполнению исполняется
 *   - Ресурсы исполненной инструкции освобождаются
 **/
class SynkExecutorDevice : public ExecutorDeviceInterface {
  public:
    SynkExecutorDevice(
        const InstructionRegistry &registry,
        RegisterRequestsManager &req_manager,
        std::mutex &m
    );

    void add_instruction(BaseInstruction i) override;

    bool has_instructions() const noexcept override;

    void handle() override;

  private:
    std::queue<GenericInstruction> in_process;
    const InstructionRegistry &registry;
    RegisterRequestsManager &req_manager;
    std::mutex &m;
};
