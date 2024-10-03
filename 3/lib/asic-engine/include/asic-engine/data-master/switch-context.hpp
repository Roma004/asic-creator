#include <cstdint>

/**
 * @brief конекст передачи данных
 *
 * Класс задаёт контекст, используемый узлами шины для восстановления маршрута
 * обратнго следования к отправителю.
 **/
class SwitchContext {
  public:
    SwitchContext();

    /**
     * @brief добавить идентификатор пройденного узла
     * @param[swc_id] -- идентификатор узла для добавления
     * @throws TODO
     * */
    void push_switch(uint8_t swc_id);

    /**
     * @brief удалить идентификатор пройденного узла
     * @param[swc_id] -- идентификатор узла для проверки валидности удаления
     * @throws TODO
     * */
    void pop_switch(uint8_t swc_id);

    /** @brief получить идентификатор последнего пройденного узла */
    uint8_t get_top_id() const noexcept;

  private:
    union {
        uint64_t data = 0;
        uint8_t swcs[8];
    };

    int get_top_idx() const noexcept;
};

