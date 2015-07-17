#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <stdint.h>
#include "register.hpp"
#include "opcode_internals.hpp"
#include "vm_state.hpp"

namespace memory
{
void Conv32BTo8B(uint32_t value, uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d);
void Conv16BTo8B(uint16_t value, uint8_t& a, uint8_t& b);
void ConvTo8B(vm_state* state, uint32_t value, uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d);

void WriteValueToMemory(vm_state* state, uint32_t location, uint32_t value);

void Push(vm_state* state, uint32_t value);
reg32 Pop(vm_state* state);
};

#endif