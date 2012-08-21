#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <string>

#define IS_BIT_TOGGLED( value, bit )	    ((value & (1 << bit)) >> bit == 1)
#define TOGGLE_BIT( value, bit )		 (value ^ (1 << bit))

#define TURN_BIT_ON( value, bit )		 (value | (1 << bit))
#define TURN_BIT_OFF( value, bit )		 (value & ~(1 << bit))

#define SET_BIT_ON( value, bit )          value = TURN_BIT_ON( value, bit )
#define SET_BIT_OFF( value, bit )         value = TURN_BIT_OFF( value, bit )

#define SIGN( value )                    ( (value) < 0 ? -1 : ((value) > 0 ? 1 : 0) )

// Converts four 8-bit integers to one 32-bit integer
#define CONV_8B_TO_32B( a, b, c, d )	((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))
// Converts two 8-bit integers to one 16-bit integer
#define CONV_8B_TO_16B( a, b )          ((uint32_t)(a) | ((uint32_t)(b) << 8))

namespace utils
{
    std::string EscapeCharacter( char input );
};

#endif