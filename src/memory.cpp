#include "opcodes.hpp"

// push reg
MAKE_OPCODE(50)
{
    state->Push(state->eax);
}
MAKE_OPCODE(51)
{
    state->Push(state->ecx);
}
MAKE_OPCODE(52)
{
    state->Push(state->edx);
}
MAKE_OPCODE(53)
{
    state->Push(state->ebx);
}
MAKE_OPCODE(54)
{
    state->Push(state->esp);
}
MAKE_OPCODE(55)
{
    state->Push(state->ebp);
}
MAKE_OPCODE(56)
{
    state->Push(state->esi);
}
MAKE_OPCODE(57)
{
    state->Push(state->edi);
}

// pop reg
MAKE_OPCODE(58)
{
    state->eax = state->Pop();
}
MAKE_OPCODE(59)
{
    state->ecx = state->Pop();
}
MAKE_OPCODE(5A)
{
    state->edx = state->Pop();
}
MAKE_OPCODE(5B)
{
    state->ebx = state->Pop();
}
MAKE_OPCODE(5C)
{
    state->esp = state->Pop();
}
MAKE_OPCODE(5D)
{
    state->ebp = state->Pop();
}
MAKE_OPCODE(5E)
{
    state->esi = state->Pop();
}
MAKE_OPCODE(5F)
{
    state->edi = state->Pop();
}

// mov reg, reg OR mov [reg+disp], reg
MAKE_OPCODE(89)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 1:
        LOG_STREAM << "[" << state->GetRegisterName(mod.reg1) << '+' << (int32_t)NEXT_INS(2) << "], "
                   << state->GetRegisterName(mod.reg2);
        state->Write(SEGMEM(state->ds, state->general[mod.reg1] + (int8_t)NEXT_INS(2)), state->general[mod.reg2]);
        op.ins_offset++;
        break;
    case 3:
        LOG_STREAM << state->GetRegisterName(mod.reg1) << ", " << state->GetRegisterName(mod.reg2);
        state->general[mod.reg1] = state->general[mod.reg2];
        break;
    };
}

// mov reg8, reg8 or mov reg8, [reg]
MAKE_OPCODE(8A)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        LOG_STREAM << state->GetByteRegisterName(mod.reg2) << ", [" << state->GetRegisterCombinationName(mod.reg1) << "]";
        GetLHRegister(state, mod.reg2) =
            MEMORY(SEGMEM(state->ds, RegisterCombinationToMemoryAddress(state, mod.reg1)));
        break;
    case 3:
        LOG_STREAM << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
        GetLHRegister(state, mod.reg1) = GetLHRegister(state, mod.reg2);
        break;
    };
}

// mov reg, [reg+disp] (note: reg1 and reg2 are swapped in the modr/m byte for some reason. wtf,
// intel?)
MAKE_OPCODE(8B)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 1)
    {
        state->general[mod.reg2] = MEMORY(SEGMEM(state->ds, state->general[mod.reg1] + (int8_t)NEXT_INS(2)));
    }
}

// mov sreg, reg
MAKE_OPCODE(8E)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 3)
    {
        LOG_STREAM << state->GetSegmentName(mod.reg2) << ", " << state->GetRegisterName(mod.reg1);
        state->segment[mod.reg2] = state->general[mod.reg1];
    }
}

// mov reg8, imm8
MAKE_OPCODE(B0)
{
    LOG_STREAM << "al, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->eax) = NEXT_INS(1);
}
MAKE_OPCODE(B1)
{
    LOG_STREAM << "cl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->ecx) = NEXT_INS(1);
}
MAKE_OPCODE(B2)
{
    LOG_STREAM << "dl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->edx) = NEXT_INS(1);
}
MAKE_OPCODE(B3)
{
    LOG_STREAM << "bl, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetLowerByte(state->ebx) = NEXT_INS(1);
}
MAKE_OPCODE(B4)
{
    LOG_STREAM << "ah, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->eax) = NEXT_INS(1);
}
MAKE_OPCODE(B5)
{
    LOG_STREAM << "ch, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->ecx) = NEXT_INS(1);
}
MAKE_OPCODE(B6)
{
    LOG_STREAM << "dh, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->edx) = NEXT_INS(1);
}
MAKE_OPCODE(B7)
{
    LOG_STREAM << "bh, " << PRINT_VALUE((uint32_t)NEXT_INS(1));
    GetUpperByte(state->ebx) = NEXT_INS(1);
}

// mov reg, immediate
MAKE_OPCODE(B8)
{
    LOG_STREAM << state->GetRegisterName(0) << ", " << PRINT_VALUE(ARG(1));
    state->eax = ARG(1);
}
MAKE_OPCODE(B9)
{
    LOG_STREAM << state->GetRegisterName(1) << ", " << PRINT_VALUE(ARG(1));
    state->ecx = ARG(1);
}
MAKE_OPCODE(BA)
{
    LOG_STREAM << state->GetRegisterName(2) << ", " << PRINT_VALUE(ARG(1));
    state->edx = ARG(1);
}
MAKE_OPCODE(BB)
{
    LOG_STREAM << state->GetRegisterName(3) << ", " << PRINT_VALUE(ARG(1));
    state->ebx = ARG(1);
}
MAKE_OPCODE(BC)
{
    LOG_STREAM << state->GetRegisterName(4) << ", " << PRINT_VALUE(ARG(1));
    state->esp = ARG(1);
}
MAKE_OPCODE(BD)
{
    LOG_STREAM << state->GetRegisterName(5) << ", " << PRINT_VALUE(ARG(1));
    state->ebp = ARG(1);
}
MAKE_OPCODE(BE)
{
    LOG_STREAM << state->GetRegisterName(6) << ", " << PRINT_VALUE(ARG(1));
    state->esi = ARG(1);
}
MAKE_OPCODE(BF)
{
    LOG_STREAM << state->GetRegisterName(7) << ", " << PRINT_VALUE(ARG(1));
    state->edi = ARG(1);
}

// mov [reg], imm8
MAKE_OPCODE(C6)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        LOG_STREAM << "[" << state->general[mod.reg1] << "], " << PRINT_VALUE((uint32_t)NEXT_INS(2))
                   << std::endl;
        state->Write(state->general[mod.reg1], NEXT_INS(2));
        break;
    case 3:
        LOG_STREAM << state->general[mod.reg1] << ", " << PRINT_VALUE((uint32_t)NEXT_INS(2));
        state->general[mod.reg1] = NEXT_INS(2);
        break;
    };
}

// mov [reg+disp], imm16/32
MAKE_OPCODE(C7)
{
    ModRM mod(NEXT_INS(1));
    ModRM sib(NEXT_INS(2)); // this is a bit of a hack, but then x86 is a bit of a hack architecture

    uint8_t displacement = 0;
    if (mod.mod == 1)
    {
        displacement = NEXT_INS(3);
        op.ins_offset++;
    }

    uint32_t memAddress = state->general[sib.reg1] + (int32_t)displacement;
    state->Write(memAddress, ARG(op.GetOffset(state) - (state->CR0.protectedMode ? 4 : 2)));
}