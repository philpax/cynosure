#include "Register.hpp"
#include "Arithmetic.hpp"
#include "VMState.hpp"
#include "Opcodes.hpp"

// cmp reg8, reg8
MAKE_OPCODE(0x38)
{
    Log << "cmp ";
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

    state->eip += 2;
}

// cmp al, imm8
MAKE_OPCODE(0x3C)
{
    Log << "cmp al, " << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    arithmetic::Sub(state, GetLowerByte(state->eax), state->ReadIPRelative(1));

    state->eip += 2;
}

// cmp eax, imm32
MAKE_OPCODE(0x3D)
{
    Log << "cmp ";
    auto argument = state->ReadImmediate(state->eip + 1);

    Log << state->GetRegisterName(0) << " " << PRINT_VALUE(argument);
    arithmetic::Sub(state, state->eax, argument);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

// jc/jb imm8
MAKE_OPCODE(0x72)
{
    Log << "jc ";
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (state->eflags.carry)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }

    state->eip += 2;
}

// jz/je imm8
MAKE_OPCODE(0x74)
{
    Log << "jz ";
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (state->eflags.zero)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }

    state->eip += 2;
}

// jnz/jne imm8
MAKE_OPCODE(0x75)
{
    Log << "jnz ";
    Log << PRINT_VALUE((int32_t)state->ReadIPRelative(1));
    if (!state->eflags.zero)
    {
        state->eip += (int8_t)state->ReadIPRelative(1);
        Log << std::endl;
    }

    state->eip += 2;
}

// nop
MAKE_OPCODE(0x90)
{
    Log << "nop";
    state->eip += 1;
}

// ret
MAKE_OPCODE(0xC3)
{
    Log << "ret";
    state->eip = state->Pop();
}

// jmp seg16:off16
MAKE_OPCODE(0xEA)
{
    Log << "jmp ";
    auto segment = state->Read<uint16_t>(state->eip + 3);
    auto offset = state->Read<uint16_t>(state->eip + 1);

    state->cs = segment;
    state->eip = segment * 16 + offset;
    Log << PRINT_VALUE(segment) << ':' << PRINT_VALUE(offset);
}

// call imm32
MAKE_OPCODE(0xE8)
{
    Log << "call ";
    auto argument = state->ReadImmediate(state->eip + 1);

    state->eip += state->CR0.protectedMode ? 5 : 3;
    state->Push(state->eip);
    state->eip += argument;
    Log << argument << std::endl;
}

// jmp imm32
MAKE_OPCODE(0xE9)
{
    Log << "jmp ";
    auto argument = state->ReadImmediate(state->eip + 1);

    if (argument == -5 || argument == -3)
        state->running = false;

    state->eip += argument;
    Log << argument << std::endl;

    state->eip += state->CR0.protectedMode ? 5 : 3;
}