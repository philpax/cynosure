#include "memory.hpp"

// Helper function
void memory::Write(VMState* state, uint32_t location, void const* data, size_t count)
{
    memcpy(&state->memory[location], data, count);
    LOG_STREAM << "[MEMORY] Wrote " << count << " bytes to " << location;
}

// Stack instructions
void memory::Push(VMState* state, uint32_t value)
{
    state->esp -= (state->CR0.protectedMode ? 4 : 2);
    LOG_STREAM << std::endl;
    Write(state, state->esp, value);

    LOG_STREAM << std::endl << "[STACK] Pushed: " << value << " to " << state->esp;
}

Register32 memory::Pop(VMState* state)
{
    Register32 value;
    value = (ARG_M(state->esp));
    LOG_STREAM << std::endl << "[STACK] Popped: " << value << " from " << state->esp;
    state->esp += (state->CR0.protectedMode ? 4 : 2);
    return value;
}

// push reg
MAKE_OPCODE(50)
{
    memory::Push(state, state->eax);
}
MAKE_OPCODE(51)
{
    memory::Push(state, state->ecx);
}
MAKE_OPCODE(52)
{
    memory::Push(state, state->edx);
}
MAKE_OPCODE(53)
{
    memory::Push(state, state->ebx);
}
MAKE_OPCODE(54)
{
    memory::Push(state, state->esp);
}
MAKE_OPCODE(55)
{
    memory::Push(state, state->ebp);
}
MAKE_OPCODE(56)
{
    memory::Push(state, state->esi);
}
MAKE_OPCODE(57)
{
    memory::Push(state, state->edi);
}

// pop reg
MAKE_OPCODE(58)
{
    state->eax = memory::Pop(state);
}
MAKE_OPCODE(59)
{
    state->ecx = memory::Pop(state);
}
MAKE_OPCODE(5A)
{
    state->edx = memory::Pop(state);
}
MAKE_OPCODE(5B)
{
    state->ebx = memory::Pop(state);
}
MAKE_OPCODE(5C)
{
    state->esp = memory::Pop(state);
}
MAKE_OPCODE(5D)
{
    state->ebp = memory::Pop(state);
}
MAKE_OPCODE(5E)
{
    state->esi = memory::Pop(state);
}
MAKE_OPCODE(5F)
{
    state->edi = memory::Pop(state);
}

// mov reg, reg OR mov [reg+disp], reg
MAKE_OPCODE(89)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 1:
        LOG_STREAM << "[" << R_Gn(mod.reg1) << '+' << (int32_t)NEXT_INS(2) << "], "
                   << R_Gn(mod.reg2);
        memory::Write(state, SEGMEM(state->ds, R_G(mod.reg1) + (int8_t)NEXT_INS(2)), R_G(mod.reg2));
        op.ins_offset++;
        break;
    case 3:
        LOG_STREAM << R_Gn(mod.reg1) << ", " << R_Gn(mod.reg2);
        R_G(mod.reg1) = R_G(mod.reg2);
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
        LOG_STREAM << R_LHn[mod.reg2] << ", [" << R_RCn(mod.reg1) << "]";
        GetLHRegister(state, mod.reg2) =
            MEMORY(SEGMEM(state->ds, RegisterCombinationToMemoryAddress(state, mod.reg1)));
        break;
    case 3:
        LOG_STREAM << R_LHn[mod.reg1] << ", " << R_LHn[mod.reg2];
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
        R_G(mod.reg2) = MEMORY(SEGMEM(state->ds, R_G(mod.reg1) + (int8_t)NEXT_INS(2)));
    }
}

// mov sreg, reg
MAKE_OPCODE(8E)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 3)
    {
        LOG_STREAM << R_Sn[mod.reg2] << ", " << R_Gn(mod.reg1);
        R_S(mod.reg2) = R_G(mod.reg1);
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
    LOG_STREAM << R_Gn(0) << ", " << PRINT_VALUE(ARG(1));
    state->eax = ARG(1);
}
MAKE_OPCODE(B9)
{
    LOG_STREAM << R_Gn(1) << ", " << PRINT_VALUE(ARG(1));
    state->ecx = ARG(1);
}
MAKE_OPCODE(BA)
{
    LOG_STREAM << R_Gn(2) << ", " << PRINT_VALUE(ARG(1));
    state->edx = ARG(1);
}
MAKE_OPCODE(BB)
{
    LOG_STREAM << R_Gn(3) << ", " << PRINT_VALUE(ARG(1));
    state->ebx = ARG(1);
}
MAKE_OPCODE(BC)
{
    LOG_STREAM << R_Gn(4) << ", " << PRINT_VALUE(ARG(1));
    state->esp = ARG(1);
}
MAKE_OPCODE(BD)
{
    LOG_STREAM << R_Gn(5) << ", " << PRINT_VALUE(ARG(1));
    state->ebp = ARG(1);
}
MAKE_OPCODE(BE)
{
    LOG_STREAM << R_Gn(6) << ", " << PRINT_VALUE(ARG(1));
    state->esi = ARG(1);
}
MAKE_OPCODE(BF)
{
    LOG_STREAM << R_Gn(7) << ", " << PRINT_VALUE(ARG(1));
    state->edi = ARG(1);
}

// mov [reg], imm8
MAKE_OPCODE(C6)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        LOG_STREAM << "[" << R_G(mod.reg1) << "], " << PRINT_VALUE((uint32_t)NEXT_INS(2))
                   << std::endl;
        memory::Write(state, R_G(mod.reg1), NEXT_INS(2));
        break;
    case 3:
        LOG_STREAM << R_G(mod.reg1) << ", " << PRINT_VALUE((uint32_t)NEXT_INS(2));
        R_G(mod.reg1) = NEXT_INS(2);
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

    uint32_t memAddress = R_G(sib.reg1) + (int32_t)displacement;
    memory::Write(state, memAddress, ARG(op.GetOffset(state) - (state->CR0.protectedMode ? 4 : 2)));
}