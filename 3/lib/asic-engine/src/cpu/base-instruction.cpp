#include <cstdint>
#include "asic-engine/cpu/base-instruction.hpp"

BaseInstruction::BaseInstruction(uint32_t inst_word) : word(inst_word) {}
