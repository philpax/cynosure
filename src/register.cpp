#include "register.hpp"
#include "vm_state.hpp"

uint8_t& GetLHRegister(vm_state* state, uint8_t index)
{
    switch (index)
    {
    case 0:
        return EAX.l;
    case 1:
        return ECX.l;
    case 2:
        return EDX.l;
    case 3:
        return EBX.l;
    case 4:
        return EAX.h;
    case 5:
        return ECX.h;
    case 6:
        return EDX.h;
    case 7:
        return EBX.h;
    };

    return EAX.l;
}

uint8_t RegisterCombinationToMemoryAddress(vm_state* state, uint8_t value)
{
    switch (value)
    {
    case 0:
        return EBX.r + ESI.r;
    case 1:
        return EBX.r + EDI.r;
    case 2:
        return EBP.r + ESI.r;
    case 3:
        return EBP.r + EDI.r;
    case 4:
        return ESI.r;
    case 5:
        return EDI.r;
    case 6:
        return EBP.r;
    case 7:
        return EBX.r;
    };

    return 0;
}