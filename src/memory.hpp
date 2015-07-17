#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <stdint.h>
#include "register.hpp"
#include "opcode_internals.hpp"
#include "VMState.hpp"

namespace memory
{
void Conv32BTo8B(uint32_t value, uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d);
void Conv16BTo8B(uint16_t value, uint8_t& a, uint8_t& b);
void ConvTo8B(VMState* state, uint32_t value, uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d);

void WriteValueToMemory(VMState* state, uint32_t location, uint32_t value);

void Push(VMState* state, uint32_t value);
Register32 Pop(VMState* state);
};

#endif