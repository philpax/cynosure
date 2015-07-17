#include "register.hpp"
#include "opcode_internals.hpp"
#include "VMState.hpp"

MAKE_OPCODE(AA)
{
    MEMORY(SEGMEM(state->es, state->edi)) = GetLowerByte(state->eax);
    state->edi += (state->eflags.direction == true ? -1 : 1);
}

MAKE_OPCODE(AC)
{
    GetLowerByte(state->eax) = MEMORY(SEGMEM(state->ds, state->esi));
    state->esi += (state->eflags.direction == true ? -1 : 1);
}