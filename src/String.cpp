#include "Register.hpp"
#include "VMState.hpp"
#include "Opcodes.hpp"

// stosb
MAKE_OPCODE(0xAA)
{
    Log << "stosb";
    state->Read<uint8_t>(state->es, state->edi) = GetLowerByte(state->eax);
    state->edi += (state->eflags.direction == true ? -1 : 1);

    state->eip += 1;
}

// lodsb
MAKE_OPCODE(0xAC)
{
    Log << "lodsb";
    GetLowerByte(state->eax) = state->Read<uint8_t>(state->ds, state->esi);
    state->esi += (state->eflags.direction == true ? -1 : 1);

    state->eip += 1;
}