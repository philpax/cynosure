#include "opcodes.hpp"

// push reg
MAKE_OPCODE(0x50)
{
    state->Push(state->eax);
}
MAKE_OPCODE(0x51)
{
    state->Push(state->ecx);
}
MAKE_OPCODE(0x52)
{
    state->Push(state->edx);
}
MAKE_OPCODE(0x53)
{
    state->Push(state->ebx);
}
MAKE_OPCODE(0x54)
{
    state->Push(state->esp);
}
MAKE_OPCODE(0x55)
{
    state->Push(state->ebp);
}
MAKE_OPCODE(0x56)
{
    state->Push(state->esi);
}
MAKE_OPCODE(0x57)
{
    state->Push(state->edi);
}

// pop reg
MAKE_OPCODE(0x58)
{
    state->eax = state->Pop();
}
MAKE_OPCODE(0x59)
{
    state->ecx = state->Pop();
}
MAKE_OPCODE(0x5A)
{
    state->edx = state->Pop();
}
MAKE_OPCODE(0x5B)
{
    state->ebx = state->Pop();
}
MAKE_OPCODE(0x5C)
{
    state->esp = state->Pop();
}
MAKE_OPCODE(0x5D)
{
    state->ebp = state->Pop();
}
MAKE_OPCODE(0x5E)
{
    state->esi = state->Pop();
}
MAKE_OPCODE(0x5F)
{
    state->edi = state->Pop();
}

// mov reg, reg OR mov [reg+disp], reg
MAKE_OPCODE(0x89)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 1:
        Log << "[" << state->GetRegisterName(mod.reg1) << '+' << (int32_t)NEXT_INS(2) << "], "
                   << state->GetRegisterName(mod.reg2);
        state->Write(SEGMEM(state->ds, state->general[mod.reg1] + (int8_t)NEXT_INS(2)), state->general[mod.reg2]);
        op.insnOffset++;
        break;
    case 3:
        Log << state->GetRegisterName(mod.reg1) << ", " << state->GetRegisterName(mod.reg2);
        state->general[mod.reg1] = state->general[mod.reg2];
        break;
    };
}

// mov reg8, reg8 or mov reg8, [reg]
MAKE_OPCODE(0x8A)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        Log << state->GetByteRegisterName(mod.reg2) << ", [" << state->GetRegisterCombinationName(mod.reg1) << "]";
        GetRegister8(state, mod.reg2) =
            MEMORY(SEGMEM(state->ds, RegisterCombinationToMemoryAddress(state, mod.reg1)));
        break;
    case 3:
        Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
        GetRegister8(state, mod.reg1) = GetRegister8(state, mod.reg2);
        break;
    };
}

// mov reg, [reg+disp] (note: reg1 and reg2 are swapped in the modr/m byte for some reason. wtf,
// intel?)
MAKE_OPCODE(0x8B)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 1)
    {
        state->general[mod.reg2] = MEMORY(SEGMEM(state->ds, state->general[mod.reg1] + (int8_t)NEXT_INS(2)));
    }
}

// mov sreg, reg
MAKE_OPCODE(0x8E)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 3)
    {
        Log << state->GetSegmentName(mod.reg2) << ", " << state->GetRegisterName(mod.reg1);
        state->segment[mod.reg2] = state->general[mod.reg1];
    }
}

// mov reg8, imm8
MAKE_OPCODE(0xB0)
{
    Log << "al, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->eax) = NEXT_INS(1);
}
MAKE_OPCODE(0xB1)
{
    Log << "cl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->ecx) = NEXT_INS(1);
}
MAKE_OPCODE(0xB2)
{
    Log << "dl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->edx) = NEXT_INS(1);
}
MAKE_OPCODE(0xB3)
{
    Log << "bl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->ebx) = NEXT_INS(1);
}
MAKE_OPCODE(0xB4)
{
    Log << "ah, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->eax) = NEXT_INS(1);
}
MAKE_OPCODE(0xB5)
{
    Log << "ch, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->ecx) = NEXT_INS(1);
}
MAKE_OPCODE(0xB6)
{
    Log << "dh, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->edx) = NEXT_INS(1);
}
MAKE_OPCODE(0xB7)
{
    Log << "bh, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->ebx) = NEXT_INS(1);
}

// mov reg, immediate
MAKE_OPCODE(0xB8)
{
    Log << state->GetRegisterName(0) << ", " << PRINT_VALUE(ARG(1));
    state->eax = ARG(1);
}
MAKE_OPCODE(0xB9)
{
    Log << state->GetRegisterName(1) << ", " << PRINT_VALUE(ARG(1));
    state->ecx = ARG(1);
}
MAKE_OPCODE(0xBA)
{
    Log << state->GetRegisterName(2) << ", " << PRINT_VALUE(ARG(1));
    state->edx = ARG(1);
}
MAKE_OPCODE(0xBB)
{
    Log << state->GetRegisterName(3) << ", " << PRINT_VALUE(ARG(1));
    state->ebx = ARG(1);
}
MAKE_OPCODE(0xBC)
{
    Log << state->GetRegisterName(4) << ", " << PRINT_VALUE(ARG(1));
    state->esp = ARG(1);
}
MAKE_OPCODE(0xBD)
{
    Log << state->GetRegisterName(5) << ", " << PRINT_VALUE(ARG(1));
    state->ebp = ARG(1);
}
MAKE_OPCODE(0xBE)
{
    Log << state->GetRegisterName(6) << ", " << PRINT_VALUE(ARG(1));
    state->esi = ARG(1);
}
MAKE_OPCODE(0xBF)
{
    Log << state->GetRegisterName(7) << ", " << PRINT_VALUE(ARG(1));
    state->edi = ARG(1);
}

// mov [reg], imm8
MAKE_OPCODE(0xC6)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        Log << "[" << state->general[mod.reg1] << "], " << PRINT_VALUE((uint32_t)NEXT_INS(2))
                   << std::endl;
        state->Write(state->general[mod.reg1], NEXT_INS(2));
        break;
    case 3:
        Log << state->general[mod.reg1] << ", " << PRINT_VALUE((uint32_t)NEXT_INS(2));
        state->general[mod.reg1] = NEXT_INS(2);
        break;
    };
}

// mov [reg+disp], imm16/32
MAKE_OPCODE(0xC7)
{
    ModRM mod(NEXT_INS(1));
    ModRM sib(NEXT_INS(2)); // this is a bit of a hack, but then x86 is a bit of a hack architecture

    uint8_t displacement = 0;
    if (mod.mod == 1)
    {
        displacement = NEXT_INS(3);
        op.insnOffset++;
    }

    uint32_t memAddress = state->general[sib.reg1] + (int32_t)displacement;
    state->Write(memAddress, ARG(op.GetOffset(state) - (state->CR0.protectedMode ? 4 : 2)));
}