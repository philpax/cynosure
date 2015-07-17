#include "Opcodes.hpp"
#include "VMState.hpp"

Opcode::Opcode()
{
}

Opcode::Opcode(Handler func, uint8_t opcode) :
    func(func),
    opcode(opcode)
{
}

void Opcode::FillTable(Opcode* opcodes)
{
    // add reg, reg
    BUILD_OPCODE(0x01);

    // add reg, [reg+disp]
    BUILD_OPCODE(0x03);

    // or reg8, reg8
    BUILD_OPCODE(0x08);

    // xor reg8, reg8
    BUILD_OPCODE(0x30);

    // cmp reg8, reg8
    BUILD_OPCODE(0x38);

    // cmp al, imm8
    BUILD_OPCODE(0x3C);

    // cmp eax, imm32
    BUILD_OPCODE(0x3D);

    // inc reg
    BUILD_OPCODE(0x40);
    BUILD_OPCODE(0x41);
    BUILD_OPCODE(0x42);
    BUILD_OPCODE(0x43);
    BUILD_OPCODE(0x44);
    BUILD_OPCODE(0x45);
    BUILD_OPCODE(0x46);
    BUILD_OPCODE(0x47);

    // dec reg
    BUILD_OPCODE(0x48);
    BUILD_OPCODE(0x49);
    BUILD_OPCODE(0x4A);
    BUILD_OPCODE(0x4B);
    BUILD_OPCODE(0x4C);
    BUILD_OPCODE(0x4D);
    BUILD_OPCODE(0x4E);
    BUILD_OPCODE(0x4F);

    // push reg
    BUILD_OPCODE(0x50);
    BUILD_OPCODE(0x51);
    BUILD_OPCODE(0x52);
    BUILD_OPCODE(0x53);
    BUILD_OPCODE(0x54);
    BUILD_OPCODE(0x55);
    BUILD_OPCODE(0x56);
    BUILD_OPCODE(0x57);

    // pop reg
    BUILD_OPCODE(0x58);
    BUILD_OPCODE(0x59);
    BUILD_OPCODE(0x5A);
    BUILD_OPCODE(0x5B);
    BUILD_OPCODE(0x5C);
    BUILD_OPCODE(0x5D);
    BUILD_OPCODE(0x5E);
    BUILD_OPCODE(0x5F);

    // jb/jc imm8
    BUILD_OPCODE(0x72);

    // jz/je imm8
    BUILD_OPCODE(0x74);

    // jnz/jne imm8
    BUILD_OPCODE(0x75);

    // add/sub/cmp reg8, imm8
    BUILD_OPCODE(0x80);

    // add/sub/cmp reg, imm16/32
    BUILD_OPCODE(0x81);

    // mov reg, reg or mov [reg+disp], reg
    BUILD_OPCODE(0x89);

    // mov reg8, reg8 or mov reg8, [reg]
    BUILD_OPCODE(0x8A);

    // mov reg, [reg+disp]
    BUILD_OPCODE(0x8B);

    // mov sreg, reg
    BUILD_OPCODE(0x8E);

    // nop
    BUILD_OPCODE(0x90);

    // stosb
    BUILD_OPCODE(0xAA);

    // lodsb
    BUILD_OPCODE(0xAC);

    // mov reg8, imm8
    BUILD_OPCODE(0xB0);
    BUILD_OPCODE(0xB1);
    BUILD_OPCODE(0xB2);
    BUILD_OPCODE(0xB3);
    BUILD_OPCODE(0xB4);
    BUILD_OPCODE(0xB5);
    BUILD_OPCODE(0xB6);
    BUILD_OPCODE(0xB7);

    // mov reg, immediate
    BUILD_OPCODE(0xB8);
    BUILD_OPCODE(0xB9);
    BUILD_OPCODE(0xBA);
    BUILD_OPCODE(0xBB);
    BUILD_OPCODE(0xBC);
    BUILD_OPCODE(0xBD);
    BUILD_OPCODE(0xBE);
    BUILD_OPCODE(0xBF);

    // rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
    BUILD_OPCODE(0xD1);

    // ret
    BUILD_OPCODE(0xC3);

    // mov [reg], imm8
    BUILD_OPCODE(0xC6);

    // mov [reg+disp], imm16/32
    BUILD_OPCODE(0xC7);

    // int imm8
    BUILD_OPCODE(0xCD);

    // jmp seg16:off16
    BUILD_OPCODE(0xEA);

    // call imm16/32
    BUILD_OPCODE(0xE8);

    // jmp imm16/32
    BUILD_OPCODE(0xE9);

    // clc
    BUILD_OPCODE(0xF8);

    // stc
    BUILD_OPCODE(0xF9);

    // inc/dec reg8
    BUILD_OPCODE(0xFE);
}