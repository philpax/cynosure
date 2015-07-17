#include "control_flow.hpp"
#include "opcodes.hpp"

// cmp reg8, reg8
MAKE_OPCODE(0x38)
{
    ModRM mod(state->ReadIPRelative(1));

    auto& reg1 = GetRegister8(state, mod.reg1);
    auto& reg2 = GetRegister8(state, mod.reg2);

    switch (mod.mod)
    {
    case 0:
        arithmetic::Sub(state, state->Read<int32_t>(state->ds, reg1), reg2);
        break;
    case 3:
        arithmetic::Sub(state, reg1, reg2);
        break;
    };
}

// cmp al, imm8
MAKE_OPCODE(0x3C)
{
    Log << "al, " << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    arithmetic::Sub(state, GetLowerByte(state->eax), state->ReadIPRelative(1));
}

// cmp eax, imm32
MAKE_OPCODE(0x3D)
{
    auto argument = state->ReadImmediate(state->eip + 1);

    Log << state->GetRegisterName(0) << " " << PRINT_VALUE(argument);
    arithmetic::Sub(state, state->eax, argument);
}

// jc/jb imm8
MAKE_OPCODE(0x72)
{
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (state->eflags.carry)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }
}

// jz/je imm8
MAKE_OPCODE(0x74)
{
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (state->eflags.zero)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }
}

// jnz/jne imm8
MAKE_OPCODE(0x75)
{
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (!state->eflags.zero)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }
}

// nop
MAKE_OPCODE(0x90)
{
}

// ret
MAKE_OPCODE(0xC3)
{
    state->eip = state->Pop();
}

// jmp seg16:off16
MAKE_OPCODE(0xEA)
{
    auto segment = state->Read<uint16_t>(state->eip + 3);
    auto offset = state->Read<uint16_t>(state->eip + 1);

    state->cs = segment;
    state->eip = segment * 16 + offset;
    Log << PRINT_VALUE(segment) << ':' << PRINT_VALUE(offset);
}

// call imm32
MAKE_OPCODE(0xE8)
{
    auto argument = state->ReadImmediate(state->eip + 1);

    state->Push(state->eip + op.GetOffset(state));
    state->eip += argument;
    Log << argument << std::endl;
}

// jmp imm32
MAKE_OPCODE(0xE9)
{
    auto argument = state->ReadImmediate(state->eip + 1);

    if (argument == -5 || argument == -3)
        state->running = false;

    state->eip += argument;
    Log << argument << std::endl;
}