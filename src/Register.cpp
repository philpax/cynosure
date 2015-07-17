#include "Register.hpp"
#include "VMState.hpp"

uint8_t& GetRegister8(VMState* state, uint8_t index)
{
    switch (index)
    {
    case 0:
        return GetLowerByte(state->eax);
    case 1:
        return GetLowerByte(state->ecx);
    case 2:
        return GetLowerByte(state->edx);
    case 3:
        return GetLowerByte(state->ebx);
    case 4:
        return GetUpperByte(state->eax);
    case 5:
        return GetUpperByte(state->ecx);
    case 6:
        return GetUpperByte(state->edx);
    case 7:
        return GetUpperByte(state->ebx);
    };

    return GetLowerByte(state->eax);
}

uint8_t RegisterCombinationToMemoryAddress(VMState* state, uint8_t value)
{
    switch (value)
    {
    case 0:
        return state->ebx + state->esi;
    case 1:
        return state->ebx + state->edi;
    case 2:
        return state->ebp + state->esi;
    case 3:
        return state->ebp + state->edi;
    case 4:
        return state->esi;
    case 5:
        return state->edi;
    case 6:
        return state->ebp;
    case 7:
        return state->ebx;
    };

    return 0;
}