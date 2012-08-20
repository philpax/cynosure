#ifndef _CONTROL_FLOW_HPP_
#define _CONTROL_FLOW_HPP_

#include <cstdint>
#include "register.hpp"
#include "arithmetic.hpp"

namespace controlflow
{
    void CallI( uint32_t value );
    void JumpI( uint32_t value );
    void JumpIfEqualI( uint8_t value );
    void CompareEAXI( uint32_t value );
    
    void Leave( );
    void Ret( );
};

#endif