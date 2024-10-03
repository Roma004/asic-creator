#pragma once
#include <asic-engine/data-master/common.hpp>


/**
 * @brief Интерфейс взаимодействия процессора с внешней шиной
 *
 * Класс задаёт минимально необходимый для функционирования процессора
 * интерфейс взаимодействия с внешней шиной.
 * */
class DataMasterPortInterface {
  public:
    virtual ~DataMasterPortInterface() {};
    /** @brief Процитать 4 байта, лежищие по заданному адресу */
    virtual uint32_t read32(addr_t) = 0;
};
