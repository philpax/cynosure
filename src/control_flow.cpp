#include "control_flow.hpp"
#include "opcodes.hpp"

// cmp reg8, reg8
MAKE_OPCODE(38)
{
    ModRM mod(NEXT_INS(1));
    switch (mod.mod)
    {
    case 0:
        arithmetic::Sub(state, MEMORY(SEGMEM(state->ds, GetLHRegister(state, mod.reg1))),
                        GetLHRegister(state, mod.reg2));
        break;
    case 3:
        arithmetic::Sub(state, GetLHRegister(state, mod.reg1), GetLHRegister(state, mod.reg2));
        break;
    };
}

// cmp al, imm8
MAKE_OPCODE(3C)
{
    LOG_STREAM << "al, " << PRINT_VALUE((int32_t)NEXT_INS(1));
    arithmetic::Sub(state, GetLowerByte(state->eax), NEXT_INS(1));
}

// cmp eax, imm32
MAKE_OPCODE(3D)
{
    LOG_STREAM << state->GetRegisterName(0) << " " << PRINT_VALUE(ARG(1));
    arithmetic::Sub(state, state->eax, ARG(1));
}

// jc/jb imm8
MAKE_OPCODE(72)
{
    LOG_STREAM << PRINT_VALUE((int32_t)NEXT_INS(1));
    if (state->eflags.carry)
    {
        state->eip += (int8_t)NEXT_INS(1);
        LOG_STREAM << std::endl;
    }
}

// jz/je imm8
MAKE_OPCODE(74)
{
    LOG_STREAM << PRINT_VALUE((int32_t)NEXT_INS(1));
    if (state->eflags.zero)
    {
        state->eip += (int8_t)NEXT_INS(1);
        LOG_STREAM << std::endl;
    }
}

// jnz/jne imm8
MAKE_OPCODE(75)
{
    LOG_STREAM << PRINT_VALUE((int32_t)NEXT_INS(1));
    if (!state->eflags.zero)
    {
        state->eip += (int8_t)NEXT_INS(1);
        LOG_STREAM << std::endl;
    }
}

// nop
MAKE_OPCODE(90)
{
}

// ret
MAKE_OPCODE(C3)
{
    state->eip = state->Pop();
}

// jmp seg16:off16
MAKE_OPCODE(EA)
{
    uint16_t segment = ARG_16B(3);
    uint16_t offset = ARG_16B(1);

    state->cs = segment;
    state->eip = SEGMEM(segment, offset);
    LOG_STREAM << PRINT_VALUE(segment) << ':' << PRINT_VALUE(offset);
}

// call imm32
MAKE_OPCODE(E8)
{
    int32_t argument = ARG(1);

    state->Push(state->eip + op.GetOffset(state));
    state->eip += argument;
    LOG_STREAM << argument << std::endl;
}

// jmp imm32
MAKE_OPCODE(E9)
{
    int32_t argument = ARG(1);

    if (argument == -5 || argument == -3)
        state->running = false;

    state->eip += argument;
    LOG_STREAM << argument << std::endl;
}