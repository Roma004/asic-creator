#pragma once
#include <asic-engine/cpu/base-instruction.hpp>
#include <asic-engine/handler-device.hpp>

/**
 * @brief Исполняющее устройство
 *
 * Класс задаёт объект, который должен исполнять, переданные от управляющего
 * утройства инструкции.
 *
 * Реализует HandlerDevice -- предполагается исполнение параллельно с остальными
 * компонентами.
 *
 * Инструкция, переданная через метод `add_instruction()` должна попасть во
 * внутреннюю очередь инструкций на исполнение, а только после этого, быть
 * от туда извлечена методом `handle()`. Это необходимо, чтобы не блокировать
 * потенциально длительной опреацией исполнения инструкции поток исполнения
 * управляющего устройства.
 *
 * На порядок исполнения инструкций не накладывается никаких ограничений, кроме
 * тех, что связаны с блокировками ресурсов, используемых переданной
 * инструкцией.
 **/
class ExecutorDeviceInterface : public HandlerDevice {
  public:
    virtual ~ExecutorDeviceInterface() = default;
    /**
     * @brief Добавить инструкцию на исполнение
     * @param[i] Инструкция, которую исполнитель должен исполнить
     */
    virtual void add_instruction(BaseInstruction i) = 0;

    /** @brief Есть ли у устройства задачи на исполнение в очереди */
    virtual bool has_instructions() const noexcept = 0;
};
