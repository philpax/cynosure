#include "arithmetic.hpp"
#include "opcodes.hpp"

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
    ModRM mod(NEXT_INS(1));
    state->general[mod.reg1] = arithmetic::Add(state, state->general[mod.reg1], state->general[mod.reg2]);
}

// add reg, [reg+disp]
MAKE_OPCODE(0x03)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 1)
    {
        state->general[mod.reg2] =
            arithmetic::Add(state, state->general[mod.reg2], MEMORY(state->general[mod.reg1] + (int8_t)NEXT_INS(2)));
    }
}

// or reg8, reg8
MAKE_OPCODE(0x08)
{
    ModRM mod(NEXT_INS(1));
    Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Or(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));
}

// xor reg8, reg8
MAKE_OPCODE(0x30)
{
    ModRM mod(NEXT_INS(1));
    Log << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Xor(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));
}

MAKE_OPCODE(0x40)
{
    state->eax = arithmetic::Add(state, state->eax, 1);
    Log << "eax";
}
MAKE_OPCODE(0x41)
{
    state->ecx = arithmetic::Add(state, state->ecx, 1);
    Log << "ecx";
}
MAKE_OPCODE(0x42)
{
    state->edx = arithmetic::Add(state, state->edx, 1);
    Log << "edx";
}
MAKE_OPCODE(0x43)
{
    state->ebx = arithmetic::Add(state, state->ebx, 1);
    Log << "ebx";
}
MAKE_OPCODE(0x44)
{
    state->esp = arithmetic::Add(state, state->esp, 1);
    Log << "esp";
}
MAKE_OPCODE(0x45)
{
    state->ebp = arithmetic::Add(state, state->ebp, 1);
    Log << "ebp";
}
MAKE_OPCODE(0x46)
{
    state->esi = arithmetic::Add(state, state->esi, 1);
    Log << "esi";
}
MAKE_OPCODE(0x47)
{
    state->edi = arithmetic::Add(state, state->edi, 1);
    Log << "edi";
}

MAKE_OPCODE(0x48)
{
    state->eax = arithmetic::Sub(state, state->eax, 1);
    Log << "eax";
}
MAKE_OPCODE(0x49)
{
    state->ecx = arithmetic::Sub(state, state->ecx, 1);
    Log << "ecx";
}
MAKE_OPCODE(0x4A)
{
    state->edx = arithmetic::Sub(state, state->edx, 1);
    Log << "edx";
}
MAKE_OPCODE(0x4B)
{
    state->ebx = arithmetic::Sub(state, state->ebx, 1);
    Log << "ebx";
}
MAKE_OPCODE(0x4C)
{
    state->esp = arithmetic::Sub(state, state->esp, 1);
    Log << "esp";
}
MAKE_OPCODE(0x4D)
{
    state->ebp = arithmetic::Sub(state, state->ebp, 1);
    Log << "ebp";
}
MAKE_OPCODE(0x4E)
{
    state->esi = arithmetic::Sub(state, state->esi, 1);
    Log << "esi";
}
MAKE_OPCODE(0x4F)
{
    state->edi = arithmetic::Sub(state, state->edi, 1);
    Log << "edi";
}

// add/sub/cmp reg8, imm8
MAKE_OPCODE(0x80)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.reg2)
    {
    case 0:
        Log << "add ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Add(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), NEXT_INS(2));
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Add(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    case 5:
        Log << "sub ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Sub(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), NEXT_INS(2));
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Sub(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    case 7:
        Log << "cmp ";
        switch (mod.mod)
        {
        case 0:
            Log << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            arithmetic::Sub(state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))),
                            NEXT_INS(2));
            break;
        case 3:
            Log << state->GetByteRegisterName(mod.reg1);
            arithmetic::Sub(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    };
}

// add/sub/cmp reg, imm16/32
MAKE_OPCODE(0x81)
{
    ModRM mod(NEXT_INS(1));

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
}

// rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
MAKE_OPCODE(0xD1)
{
    ModRM mod(NEXT_INS(1));
    switch (mod.reg2)
    {
    case 4:
        state->general[mod.reg1] = arithmetic::ShiftLeft(state, state->general[mod.reg1], 1);
        break;
    };
}

// clc
MAKE_OPCODE(0xF8)
{
    state->eflags.carry = false;
}

// stc
MAKE_OPCODE(0xF9)
{
    state->eflags.carry = true;
}

// inc/dec cl
MAKE_OPCODE(0xFE)
{
    ModRM mod(NEXT_INS(1));
    switch (mod.reg2)
    {
    case 0:
        state->general[mod.reg1] = arithmetic::Add(state, mod.reg1, 1);
        break;
    case 1:
        state->general[mod.reg1] = arithmetic::Sub(state, mod.reg1, 1);
        break;
    };
}