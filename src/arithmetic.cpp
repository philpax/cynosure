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
MAKE_OPCODE(01)
{
    ModRM mod(NEXT_INS(1));
    state->general[mod.reg1] = arithmetic::Add(state, state->general[mod.reg1], state->general[mod.reg2]);
}

// add reg, [reg+disp]
MAKE_OPCODE(03)
{
    ModRM mod(NEXT_INS(1));
    if (mod.mod == 1)
    {
        state->general[mod.reg2] =
            arithmetic::Add(state, state->general[mod.reg2], MEMORY(state->general[mod.reg1] + (int8_t)NEXT_INS(2)));
    }
}

// or reg8, reg8
MAKE_OPCODE(08)
{
    ModRM mod(NEXT_INS(1));
    LOG_STREAM << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Or(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));
}

// xor reg8, reg8
MAKE_OPCODE(30)
{
    ModRM mod(NEXT_INS(1));
    LOG_STREAM << state->GetByteRegisterName(mod.reg1) << ", " << state->GetByteRegisterName(mod.reg2);
    GetRegister8(state, mod.reg1) =
        arithmetic::Xor(state, GetRegister8(state, mod.reg1), GetRegister8(state, mod.reg2));
}

MAKE_OPCODE(40)
{
    state->eax = arithmetic::Add(state, state->eax, 1);
    LOG_STREAM << "eax";
}
MAKE_OPCODE(41)
{
    state->ecx = arithmetic::Add(state, state->ecx, 1);
    LOG_STREAM << "ecx";
}
MAKE_OPCODE(42)
{
    state->edx = arithmetic::Add(state, state->edx, 1);
    LOG_STREAM << "edx";
}
MAKE_OPCODE(43)
{
    state->ebx = arithmetic::Add(state, state->ebx, 1);
    LOG_STREAM << "ebx";
}
MAKE_OPCODE(44)
{
    state->esp = arithmetic::Add(state, state->esp, 1);
    LOG_STREAM << "esp";
}
MAKE_OPCODE(45)
{
    state->ebp = arithmetic::Add(state, state->ebp, 1);
    LOG_STREAM << "ebp";
}
MAKE_OPCODE(46)
{
    state->esi = arithmetic::Add(state, state->esi, 1);
    LOG_STREAM << "esi";
}
MAKE_OPCODE(47)
{
    state->edi = arithmetic::Add(state, state->edi, 1);
    LOG_STREAM << "edi";
}

MAKE_OPCODE(48)
{
    state->eax = arithmetic::Sub(state, state->eax, 1);
    LOG_STREAM << "eax";
}
MAKE_OPCODE(49)
{
    state->ecx = arithmetic::Sub(state, state->ecx, 1);
    LOG_STREAM << "ecx";
}
MAKE_OPCODE(4A)
{
    state->edx = arithmetic::Sub(state, state->edx, 1);
    LOG_STREAM << "edx";
}
MAKE_OPCODE(4B)
{
    state->ebx = arithmetic::Sub(state, state->ebx, 1);
    LOG_STREAM << "ebx";
}
MAKE_OPCODE(4C)
{
    state->esp = arithmetic::Sub(state, state->esp, 1);
    LOG_STREAM << "esp";
}
MAKE_OPCODE(4D)
{
    state->ebp = arithmetic::Sub(state, state->ebp, 1);
    LOG_STREAM << "ebp";
}
MAKE_OPCODE(4E)
{
    state->esi = arithmetic::Sub(state, state->esi, 1);
    LOG_STREAM << "esi";
}
MAKE_OPCODE(4F)
{
    state->edi = arithmetic::Sub(state, state->edi, 1);
    LOG_STREAM << "edi";
}

// add/sub/cmp reg8, imm8
MAKE_OPCODE(80)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.reg2)
    {
    case 0:
        LOG_STREAM << "add ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Add(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), NEXT_INS(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Add(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    case 5:
        LOG_STREAM << "sub ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Sub(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), NEXT_INS(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Sub(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    case 7:
        LOG_STREAM << "cmp ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            arithmetic::Sub(state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))),
                            NEXT_INS(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            arithmetic::Sub(state, GetRegister8(state, mod.reg1), NEXT_INS(2));
            break;
        };
        break;
    };
}

// add/sub/cmp reg, imm16/32
MAKE_OPCODE(81)
{
    ModRM mod(NEXT_INS(1));

    switch (mod.reg2)
    {
    case 0:
        LOG_STREAM << "add ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Add(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), ARG(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Add(state, GetRegister8(state, mod.reg1), ARG(2));
            break;
        };
        break;
    case 5:
        LOG_STREAM << "sub ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))) = arithmetic::Sub(
                state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))), ARG(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            GetRegister8(state, mod.reg1) =
                arithmetic::Sub(state, GetRegister8(state, mod.reg1), ARG(2));
            break;
        };
        break;
    case 7:
        LOG_STREAM << "cmp ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << state->GetByteRegisterName(mod.reg1) << ']';
            arithmetic::Sub(state, MEMORY(SEGMEM(state->ds, GetRegister8(state, mod.reg1))),
                            ARG(2));
            break;
        case 3:
            LOG_STREAM << state->GetByteRegisterName(mod.reg1);
            arithmetic::Sub(state, GetRegister8(state, mod.reg1), ARG(2));
            break;
        };
        break;
    };

    LOG_STREAM << ", " << PRINT_VALUE(ARG(2));
}

// rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
MAKE_OPCODE(D1)
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
MAKE_OPCODE(F8)
{
    state->eflags.carry = false;
}

// stc
MAKE_OPCODE(F9)
{
    state->eflags.carry = true;
}

// inc/dec cl
MAKE_OPCODE(FE)
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