#pragma once

#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <string>

#define IS_BIT_TOGGLED(value, bit) ((value & (1 << bit)) >> bit == 1)
#define TOGGLE_BIT(value, bit) (value ^ (1 << bit))

#define TURN_BIT_ON(value, bit) (value | (1 << bit))
#define TURN_BIT_OFF(value, bit) (value & ~(1 << bit))

#define SET_BIT_ON(value, bit) value = TURN_BIT_ON(value, bit)
#define SET_BIT_OFF(value, bit) value = TURN_BIT_OFF(value, bit)

#define SIGN(value) ((value) < 0 ? -1 : ((value) > 0 ? 1 : 0))

// Provides a hexadecimal value with correct padding, etc for printing
#define PRINT_VALUE(value)                                                                         \
    "0x" << std::hex << std::uppercase << (state->CR0.protectedMode ? std::setw(8) : std::setw(4)) \
         << std::right << std::setfill('0') << (value)

namespace utils
{
    std::string EscapeCharacter(char input);
};