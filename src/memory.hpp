#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <stdint.h>
#include "register.hpp"
#include "opcode_internals.hpp"
#include "VMState.hpp"

namespace memory
{
    void Write(VMState* state, uint32_t location, void const* data, size_t count);

    template <typename T>
    void Write(VMState* state, uint32_t location, T const& value)
    {
        Write(state, location, &value, sizeof(value));
    }

    template <typename T>
    T& Read(VMState* state, uint32_t location)
    {
        return *reinterpret_cast<T*>(&state->memory[location]);
    }

    int32_t ReadImmediate(VMState* state, uint32_t location);

    void Push(VMState* state, uint32_t value);
    uint32_t Pop(VMState* state);
};

#endif