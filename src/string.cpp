#include "register.hpp"
#include "opcode_internals.hpp"
#include "VMState.hpp"

MAKE_OPCODE(AA)
{
    MEMORY(SEGMEM(ES.r, EDI.r)) = EAX.l;
    EDI.r += (EFLAGS.direction == true ? -1 : 1);
}

MAKE_OPCODE(AC)
{
    EAX.l = MEMORY(SEGMEM(DS.r, ESI.r));
    ESI.r += (EFLAGS.direction == true ? -1 : 1);
}