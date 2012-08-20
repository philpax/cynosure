#ifndef _ARITHMETIC_HPP_
#define _ARITHMETIC_HPP_

#include <cstdint>
#include "register.hpp"

namespace arithmetic
{
    uint32_t Add( vm_state *state, uint32_t a, uint32_t b );
    uint32_t Sub( vm_state *state, uint32_t a, uint32_t b );

    uint32_t And( vm_state *state, uint32_t a, uint32_t b );
    uint32_t Or( vm_state *state, uint32_t a, uint32_t b );
    uint32_t Xor( vm_state *state, uint32_t a, uint32_t b );

    uint32_t ShiftLeft( vm_state *state, uint32_t a, uint32_t b );
};

#endif