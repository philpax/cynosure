#include "Opcodes.hpp"
#include "VMState.hpp"

// push reg
MAKE_OPCODE(0x50)
{
    Log << "push ";
    state->Push(state->eax);
    state->eip += 1;
}
MAKE_OPCODE(0x51)
{
    Log << "push ";
    state->Push(state->ecx);
    state->eip += 1;
}
MAKE_OPCODE(0x52)
{
    Log << "push ";
    state->Push(state->edx);
    state->eip += 1;
}
MAKE_OPCODE(0x53)
{
    Log << "push ";
    state->Push(state->ebx);
    state->eip += 1;
}
MAKE_OPCODE(0x54)
{
    Log << "push ";
    state->Push(state->esp);
    state->eip += 1;
}
MAKE_OPCODE(0x55)
{
    Log << "push ";
    state->Push(state->ebp);
    state->eip += 1;
}
MAKE_OPCODE(0x56)
{
    Log << "push ";
    state->Push(state->esi);
    state->eip += 1;
}
MAKE_OPCODE(0x57)
{
    Log << "push ";
    state->Push(state->edi);
    state->eip += 1;
}

// pop reg
MAKE_OPCODE(0x58)
{
    Log << "pop ";
    state->eax = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x59)
{
    Log << "pop ";
    state->ecx = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5A)
{
    Log << "pop ";
    state->edx = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5B)
{
    Log << "pop ";
    state->ebx = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5C)
{
    Log << "pop ";
    state->esp = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5D)
{
    Log << "pop ";
    state->ebp = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5E)
{
    Log << "pop ";
    state->esi = state->Pop();
    state->eip += 1;
}
MAKE_OPCODE(0x5F)
{
    Log << "pop ";
    state->edi = state->Pop();
    state->eip += 1;
}

// mov reg, reg OR mov [reg+disp], reg
MAKE_OPCODE(0x89)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));

    auto offset = 2;

    switch (mod.mod)
    {
    case 1:
    {
        auto disp = static_cast<int32_t>(state->ReadIPRelative(2));
        Log << "[" << state->GetRegisterName(mod.reg1) << '+' << disp << "], "
                   << state->GetRegisterName(mod.reg2);
        state->Write(state->ds * 16 + (state->general[mod.reg1] + disp), state->general[mod.reg2]);
        offset++;
        break;
    }
    case 3:
        Log << state->GetRegisterName(mod.reg1) << ", " << state->GetRegisterName(mod.reg2);
        state->general[mod.reg1] = state->general[mod.reg2];
        break;
    };

    state->eip += offset;
}

// mov reg8, reg8 or mov reg8, [reg]
MAKE_OPCODE(0x8A)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));

    switch (mod.mod)
    {
    case 0:
        Log << state->GetByteRegisterName(mod.reg2) << ", [" << state->GetRegisterCombinationName(mod.reg1) << "]";
        GetRegister8(state, mod.reg2) =
            state->Read<uint8_t>(state->ds, RegisterCombinationToMemoryAddress(state, mod.reg1));
        break;
    case 3:
        Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
        GetRegister8(state, mod.reg1) = GetRegister8(state, mod.reg2);
        break;
    };

    state->eip += 2;
}

// mov reg, [reg+disp]
MAKE_OPCODE(0x8B)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));

    if (mod.mod == 1)
    {
        auto offset = state->general[mod.reg1] + (int8_t)state->ReadIPRelative(2);
        state->general[mod.reg2] = state->Read<uint32_t>(state->ds, offset);
    }

    state->eip += 3;
}

// mov sreg, reg
MAKE_OPCODE(0x8E)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));
    if (mod.mod == 3)
    {
        Log << state->GetSegmentName(mod.reg2) << ", " << state->GetRegisterName(mod.reg1);
        state->segment[mod.reg2] = state->general[mod.reg1];
    }

    state->eip += 2;
}

// mov reg8, imm8
MAKE_OPCODE(0xB0)
{
    Log << "mov al, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetLowerByte(state->eax) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB1)
{
    Log << "mov cl, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetLowerByte(state->ecx) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB2)
{
    Log << "mov dl, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetLowerByte(state->edx) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB3)
{
    Log << "mov bl, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetLowerByte(state->ebx) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB4)
{
    Log << "mov ah, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetUpperByte(state->eax) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB5)
{
    Log << "mov ch, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetUpperByte(state->ecx) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB6)
{
    Log << "mov dh, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetUpperByte(state->edx) = state->ReadIPRelative(1);

    state->eip += 2;
}
MAKE_OPCODE(0xB7)
{
    Log << "mov bh, " << PRINT_VALUE(static_cast<uint32_t>(state->ReadIPRelative(1)));
    GetUpperByte(state->ebx) = state->ReadIPRelative(1);

    state->eip += 2;
}

// mov reg, immediate
MAKE_OPCODE(0xB8)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->eax = value;
    Log << state->GetRegisterName(0) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xB9)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->ecx = value;
    Log << state->GetRegisterName(1) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBA)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->edx = value;
    Log << state->GetRegisterName(2) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBB)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->ebx = value;
    Log << state->GetRegisterName(3) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBC)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->esp = value;
    Log << state->GetRegisterName(4) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBD)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->ebp = value;
    Log << state->GetRegisterName(5) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBE)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->esi = value;
    Log << state->GetRegisterName(6) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

MAKE_OPCODE(0xBF)
{
    Log << "mov ";
    auto value = state->ReadImmediate(state->eip + 1);
    state->edi = value;
    Log << state->GetRegisterName(7) << ", " << PRINT_VALUE(value);

    state->eip += state->CR0.protectedMode ? 5 : 3;
}

// mov [reg], imm8
MAKE_OPCODE(0xC6)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));

    auto immediate = state->ReadIPRelative(2);

    switch (mod.mod)
    {
    case 0:
        Log << "[" << state->general[mod.reg1] << "], "
                   << PRINT_VALUE(static_cast<uint32_t>(immediate))
                   << std::endl;

        state->Write(state->general[mod.reg1], immediate);
        break;
    case 3:
        Log << state->general[mod.reg1] << ", "
            << PRINT_VALUE(static_cast<uint32_t>(immediate))
            << std::endl;

        state->general[mod.reg1] = immediate;
        break;
    };

    state->eip += 3;
}

// mov [reg+disp], imm16/32
MAKE_OPCODE(0xC7)
{
    Log << "mov ";
    ModRM mod(state->ReadIPRelative(1));
    ModRM sib(state->ReadIPRelative(2));

    auto insOffset = state->CR0.protectedMode ? 7 : 5;

    uint8_t displacement = 0;
    if (mod.mod == 1)
    {
        displacement = state->ReadIPRelative(3);
        insOffset++;
    }

    uint32_t memAddress = state->general[sib.reg1] + static_cast<int32_t>(displacement);
    auto offset = insOffset - (state->CR0.protectedMode ? 4 : 2);
    auto immediate = state->ReadImmediate(state->eip + offset);

    if (state->CR0.protectedMode)
        state->Write<uint32_t>(memAddress, immediate);
    else
        state->Write<uint16_t>(memAddress, immediate);

    state->eip += insOffset;
}