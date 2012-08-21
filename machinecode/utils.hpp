#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <stdint.h>

#define IS_BIT_TOGGLED( value, bit )	    ((value & (1 << bit)) >> bit == 1)
#define TOGGLE_BIT( value, bit )		 (value ^ (1 << bit))

#define TURN_BIT_ON( value, bit )		 (value | (1 << bit))
#define TURN_BIT_OFF( value, bit )		 (value & ~(1 << bit))

#define SET_BIT_ON( value, bit )          value = TURN_BIT_ON( value, bit )
#define SET_BIT_OFF( value, bit )         value = TURN_BIT_OFF( value, bit )

#define SIGN( value )                    ( (value) < 0 ? -1 : ((value) > 0 ? 1 : 0) )

#endif