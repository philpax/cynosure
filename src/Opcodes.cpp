#include "Opcodes.hpp"
#include "VMState.hpp"

Opcode::Opcode()
{
}

Opcode::Opcode(Handler func, uint8_t opcode, std::string name, uint8_t offset32, uint8_t offset16):
    func(func),
    opcode(opcode),
    name(name),
    offset32(offset32),
    offset16(offset16)
{
}

uint8_t Opcode::GetOffset(VMState* state)
{
    if (state->CR0.protectedMode)
        return offset32 + insnOffset;
    else
        return offset16 + insnOffset;
}

uint8_t Opcode::GetFinalOffset(VMState* state)
{
    uint8_t offset = GetOffset(state);
    insnOffset = 0;
    return offset;
}

void Opcode::FillTable(Opcode* opcodes)
{
    // add reg, reg
    DECL_OPCODE(0x01, "add ", 2, 2);

    // add reg, [reg+disp]
    DECL_OPCODE(0x03, "add ", 3, 3);

    // or reg8, reg8
    DECL_OPCODE(0x08, "or ", 2, 2);

    // xor reg8, reg8
    DECL_OPCODE(0x30, "xor ", 2, 2);

    // cmp reg8, reg8
    DECL_OPCODE(0x38, "cmp ", 2, 2);

    // cmp al, imm8
    DECL_OPCODE(0x3C, "cmp ", 2, 2);

    // cmp eax, imm32
    DECL_OPCODE(0x3D, "cmp ", 5, 3);

    // inc reg
    DECL_OPCODE(0x40, "inc ", 1, 1);
    DECL_OPCODE(0x41, "inc ", 1, 1);
    DECL_OPCODE(0x42, "inc ", 1, 1);
    DECL_OPCODE(0x43, "inc ", 1, 1);
    DECL_OPCODE(0x44, "inc ", 1, 1);
    DECL_OPCODE(0x45, "inc ", 1, 1);
    DECL_OPCODE(0x46, "inc ", 1, 1);
    DECL_OPCODE(0x47, "inc ", 1, 1);

    // dec reg
    DECL_OPCODE(0x48, "dec ", 1, 1);
    DECL_OPCODE(0x49, "dec ", 1, 1);
    DECL_OPCODE(0x4A, "dec ", 1, 1);
    DECL_OPCODE(0x4B, "dec ", 1, 1);
    DECL_OPCODE(0x4C, "dec ", 1, 1);
    DECL_OPCODE(0x4D, "dec ", 1, 1);
    DECL_OPCODE(0x4E, "dec ", 1, 1);
    DECL_OPCODE(0x4F, "dec ", 1, 1);

    // push reg
    DECL_OPCODE(0x50, "push ", 1, 1);
    DECL_OPCODE(0x51, "push ", 1, 1);
    DECL_OPCODE(0x52, "push ", 1, 1);
    DECL_OPCODE(0x53, "push ", 1, 1);
    DECL_OPCODE(0x54, "push ", 1, 1);
    DECL_OPCODE(0x55, "push ", 1, 1);
    DECL_OPCODE(0x56, "push ", 1, 1);
    DECL_OPCODE(0x57, "push ", 1, 1);

    // pop reg
    DECL_OPCODE(0x58, "pop ", 1, 1);
    DECL_OPCODE(0x59, "pop ", 1, 1);
    DECL_OPCODE(0x5A, "pop ", 1, 1);
    DECL_OPCODE(0x5B, "pop ", 1, 1);
    DECL_OPCODE(0x5C, "pop ", 1, 1);
    DECL_OPCODE(0x5D, "pop ", 1, 1);
    DECL_OPCODE(0x5E, "pop ", 1, 1);
    DECL_OPCODE(0x5F, "pop ", 1, 1);

    // jb/jc imm8
    DECL_OPCODE(0x72, "jb ", 2, 2);

    // jz/je imm8
    DECL_OPCODE(0x74, "jz ", 2, 2);

    // jnz/jne imm8
    DECL_OPCODE(0x75, "jnz ", 2, 2);

    // add/sub/cmp reg8, imm8
    DECL_OPCODE(0x80, "", 3, 3);

    // add/sub/cmp reg, imm16/32
    DECL_OPCODE(0x81, "add/sub/cmp ", 6, 4);

    // mov reg, reg or mov [reg+disp], reg
    DECL_OPCODE(0x89, "mov ", 2, 2);

    // mov reg8, reg8 or mov reg8, [reg]
    DECL_OPCODE(0x8A, "mov ", 2, 2);

    // mov reg, [reg+disp]
    DECL_OPCODE(0x8B, "mov ", 3, 3);

    // mov sreg, reg
    DECL_OPCODE(0x8E, "mov ", 2, 2);

    // nop
    DECL_OPCODE(0x90, "nop", 1, 1);

    // stosb
    DECL_OPCODE(0xAA, "stosb ", 1, 1);

    // lodsb
    DECL_OPCODE(0xAC, "lodsb ", 1, 1);

    // mov reg8, imm8
    DECL_OPCODE(0xB0, "mov ", 2, 2);
    DECL_OPCODE(0xB1, "mov ", 2, 2);
    DECL_OPCODE(0xB2, "mov ", 2, 2);
    DECL_OPCODE(0xB3, "mov ", 2, 2);
    DECL_OPCODE(0xB4, "mov ", 2, 2);
    DECL_OPCODE(0xB5, "mov ", 2, 2);
    DECL_OPCODE(0xB6, "mov ", 2, 2);
    DECL_OPCODE(0xB7, "mov ", 2, 2);

    // mov reg, immediate
    DECL_OPCODE(0xB8, "mov ", 5, 3);
    DECL_OPCODE(0xB9, "mov ", 5, 3);
    DECL_OPCODE(0xBA, "mov ", 5, 3);
    DECL_OPCODE(0xBB, "mov ", 5, 3);
    DECL_OPCODE(0xBC, "mov ", 5, 3);
    DECL_OPCODE(0xBD, "mov ", 5, 3);
    DECL_OPCODE(0xBE, "mov ", 5, 3);
    DECL_OPCODE(0xBF, "mov ", 5, 3);

    // rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
    DECL_OPCODE(0xD1, "rol/ror/rcl/rcr/shl/shr/shl/sar ", 2, 2);

    // ret
    DECL_OPCODE(0xC3, "ret ", 0, 0);

    // mov [reg], imm8
    DECL_OPCODE(0xC6, "mov ", 3, 3);

    // mov [reg+disp], imm16/32
    DECL_OPCODE(0xC7, "mov ", 7, 5);

    // int imm8
    DECL_OPCODE(0xCD, "int ", 2, 2);

    // jmp seg16:off16
    DECL_OPCODE(0xEA, "jmp ", 0, 0);

    // call imm16/32
    DECL_OPCODE(0xE8, "call ", 5, 3);

    // jmp imm16/32
    DECL_OPCODE(0xE9, "jmp ", 5, 3);

    // clc
    DECL_OPCODE(0xF8, "clc ", 1, 1);

    // stc
    DECL_OPCODE(0xF9, "stc ", 1, 1);

    // inc/dec reg8
    DECL_OPCODE(0xFE, "inc/dec ", 2, 2);
}