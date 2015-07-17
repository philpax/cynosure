#include "Arithmetic.hpp"
#include "Opcodes.hpp"

void UpdateEFLAGS(VMState* state, uint32_t value)
{
    state->eflags.zero = value == 0;
    state->eflags.sign = static_cast<int32_t>(value) < 0;
}

uint32_t arithmetic::Add(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a + b;
    UpdateEFLAGS(state, value);

    return value;
}

uint32_t arithmetic::Sub(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a - b;
    UpdateEFLAGS(state, value);

    return value;
}

uint32_t arithmetic::And(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a & b;
    UpdateEFLAGS(state, value);

    state->eflags.overflow = false;
    state->eflags.carry = false;

    return value;
}

uint32_t arithmetic::Or(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a | b;
    UpdateEFLAGS(state, value);

    state->eflags.overflow = false;
    state->eflags.carry = false;

    return value;
}

uint32_t arithmetic::Xor(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a ^ b;
    UpdateEFLAGS(state, value);

    state->eflags.overflow = false;
    state->eflags.carry = false;

    return value;
}

uint32_t arithmetic::ShiftLeft(VMState* state, uint32_t a, uint32_t b)
{
    uint32_t value = a << b;
    UpdateEFLAGS(state, value);

    state->eflags.overflow = false;
    state->eflags.carry = false;

    return value;
}

// add reg, reg
MAKE_OPCODE(0x01)
{
    Log << "add ";

    ModRM mod(state->ReadIPRelative(1));
    state->general[mod.reg1] = arithmetic::Add(state, state->general[mod.reg1], state->general[mod.reg2]);

    state->eip += 2;
}

// add reg, [reg+disp]
MAKE_OPCODE(0x03)
{
    Log << "add ";

    ModRM mod(state->ReadIPRelative(1));
    if (mod.mod == 1)
    {
        auto& lhs = state->general[mod.reg2];
        auto rhs = state->memory[state->general[mod.reg1] + (int8_t)state->ReadIPRelative(2)];
        lhs = arithmetic::Add(state, lhs, rhs);
    }

    state->eip += 3;
}

// or reg8, reg8
MAKE_OPCODE(0x08)
{
    Log << "or ";

    ModRM mod(state->ReadIPRelative(1));
    Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Or(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));

    state->eip += 2;
}

// xor reg8, reg8
MAKE_OPCODE(0x30)
{
    Log << "xor ";

    ModRM mod(state->ReadIPRelative(1));
    Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Xor(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));

    state->eip += 2;
}

MAKE_OPCODE(0x40)
{
    Log << "add eax, 1";
    state->eax = arithmetic::Add(state, state->eax, 1);

    state->eip += 1;
}

MAKE_OPCODE(0x41)
{
    state->ecx = arithmetic::Add(state, state->ecx, 1);
    Log << "add ecx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x42)
{
    state->edx = arithmetic::Add(state, state->edx, 1);
    Log << "add edx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x43)
{
    state->ebx = arithmetic::Add(state, state->ebx, 1);
    Log << "add ebx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x44)
{
    state->esp = arithmetic::Add(state, state->esp, 1);
    Log << "add esp, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x45)
{
    state->ebp = arithmetic::Add(state, state->ebp, 1);
    Log << "add ebp, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x46)
{
    state->esi = arithmetic::Add(state, state->esi, 1);
    Log << "add esi, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x47)
{
    state->edi = arithmetic::Add(state, state->edi, 1);
    Log << "add edi, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x48)
{
    state->eax = arithmetic::Sub(state, state->eax, 1);
    Log << "sub eax, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x49)
{
    state->ecx = arithmetic::Sub(state, state->ecx, 1);
    Log << "sub ecx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4A)
{
    state->edx = arithmetic::Sub(state, state->edx, 1);
    Log << "sub edx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4B)
{
    state->ebx = arithmetic::Sub(state, state->ebx, 1);
    Log << "sub ebx, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4C)
{
    state->esp = arithmetic::Sub(state, state->esp, 1);
    Log << "sub esp, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4D)
{
    state->ebp = arithmetic::Sub(state, state->ebp, 1);
    Log << "sub ebp, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4E)
{
    state->esi = arithmetic::Sub(state, state->esi, 1);
    Log << "sub esi, 1";

    state->eip += 1;
}

MAKE_OPCODE(0x4F)
{
    state->edi = arithmetic::Sub(state, state->edi, 1);
    Log << "sub edi, 1";

    state->eip += 1;
}

// add/sub/cmp reg8, imm8
MAKE_OPCODE(0x80)
{
    ModRM mod(state->ReadIPRelative(1));

    auto immediate = state->Read<uint8_t>(state->eip + 2);
    auto& reg1 = GetRegister8(state, mod.reg1);
    auto& value = state->Read<int32_t>(state->ds, reg1);

    switch (mod.reg2)
    {
    case 0:
        Log << "add ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            value = arithmetic::Add(state, value, immediate);
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            reg1 = arithmetic::Add(state, reg1, immediate);
            break;
        };
        break;
    case 5:
        Log << "sub ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            value = arithmetic::Sub(state, value, immediate);
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            reg1 = arithmetic::Sub(state, reg1, immediate);
            break;
        };
        break;
    case 7:
        Log << "cmp ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            arithmetic::Sub(state, value, immediate);
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            arithmetic::Sub(state, reg1, immediate);
            break;
        };
        break;
    };

    state->eip += 3;
}

// add/sub/cmp reg, imm16/32
MAKE_OPCODE(0x81)
{
    ModRM mod(state->ReadIPRelative(1));

    auto immediate = state->ReadImmediate(state->eip + 2);
    auto& value = state->Read<int32_t>(state->ds, GetRegister8(state, mod.reg1));

    switch (mod.reg2)
    {
    case 0:
        Log << "add ";
        if (mod.mod == 0)
        {
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            value = arithmetic::Add(state, value, immediate);
        }
        else if (mod.mod == 3)
        {
            Log << state->GetByteRegisterName(mod.reg1);
            value = arithmetic::Add(state, value, immediate);
            break;
        }
        break;
    case 5:
        Log << "sub ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            value = arithmetic::Sub(state, value, immediate);
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            value = arithmetic::Sub(state, value, immediate);
            break;
        };
        break;
    case 7:
        Log << "cmp ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            arithmetic::Sub(state, value, immediate);
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            arithmetic::Sub(state, value, immediate);
            break;
        };
        break;
    };

    Log << ", " << PRINT_VALUE(immediate);

    state->eip += state->CR0.protectedMode ? 6 : 4;
}

// rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
MAKE_OPCODE(0xD1)
{
    ModRM mod(state->ReadIPRelative(1));
    switch (mod.reg2)
    {
    case 4:
        Log << "shl ";
        state->general[mod.reg1] = arithmetic::ShiftLeft(state, state->general[mod.reg1], 1);
        break;
    };

    state->eip += 2;
}

// clc
MAKE_OPCODE(0xF8)
{
    Log << "clc";
    state->eflags.carry = false;

    state->eip += 1;
}

// stc
MAKE_OPCODE(0xF9)
{
    Log << "stc";
    state->eflags.carry = true;

    state->eip += 1;
}

// inc/dec cl
MAKE_OPCODE(0xFE)
{
    ModRM mod(state->ReadIPRelative(1));
    switch (mod.reg2)
    {
    case 0:
        Log << "inc";
        state->general[mod.reg1] = arithmetic::Add(state, mod.reg1, 1);
        break;
    case 1:
        Log << "dec";
        state->general[mod.reg1] = arithmetic::Sub(state, mod.reg1, 1);
        break;
    };

    state->eip += 2;
}