#include "register.hpp"
#include "VMState.hpp"
#include "opcodes.hpp"

MAKE_OPCODE(0xAA)
{
    state->Read<uint8_t>(state->es, state->edi) = GetLowerByte(state->eax);
    state->edi += (state->eflags.direction == true ? -1 : 1);
}

MAKE_OPCODE(0xAC)
{
    GetLowerByte(state->eax) = state->Read<uint8_t>(state->ds, state->esi);
    state->esi += (state->eflags.direction == true ? -1 : 1);
}