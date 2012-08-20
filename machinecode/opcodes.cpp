#include "opcodes.hpp"

void opcodesGenerate( opcode *opcodes )
{
    // add reg, reg
    DECL_OPCODE( 01, "add ", 2, 2 );

    // add reg, [reg+disp]
    DECL_OPCODE( 03, "add ", 3, 3 );

    // or reg8, reg8
    DECL_OPCODE( 08, "or ", 2, 2 );

    // xor reg8, reg8
    DECL_OPCODE( 30, "xor ", 2, 2 );

    // cmp reg8, reg8
    DECL_OPCODE( 38, "cmp ", 2, 2 );

    // cmp al, imm8
    DECL_OPCODE( 3C, "cmp ", 2, 2 );

    // cmp eax, imm32
    DECL_OPCODE( 3D, "cmp ", 5, 3 );

    // inc reg
    DECL_OPCODE( 40, "inc ", 1, 1 );
    DECL_OPCODE( 41, "inc ", 1, 1 );
    DECL_OPCODE( 42, "inc ", 1, 1 );
    DECL_OPCODE( 43, "inc ", 1, 1 );
    DECL_OPCODE( 44, "inc ", 1, 1 );
    DECL_OPCODE( 45, "inc ", 1, 1 );
    DECL_OPCODE( 46, "inc ", 1, 1 );
    DECL_OPCODE( 47, "inc ", 1, 1 );

    // dec reg
    DECL_OPCODE( 48, "dec ", 1, 1 );
    DECL_OPCODE( 49, "dec ", 1, 1 );
    DECL_OPCODE( 4A, "dec ", 1, 1 );
    DECL_OPCODE( 4B, "dec ", 1, 1 );
    DECL_OPCODE( 4C, "dec ", 1, 1 );
    DECL_OPCODE( 4D, "dec ", 1, 1 );
    DECL_OPCODE( 4E, "dec ", 1, 1 );
    DECL_OPCODE( 4F, "dec ", 1, 1 );

    // push reg
    DECL_OPCODE( 50, "push ", 1, 1 );
    DECL_OPCODE( 51, "push ", 1, 1 );
    DECL_OPCODE( 52, "push ", 1, 1 );
    DECL_OPCODE( 53, "push ", 1, 1 );
    DECL_OPCODE( 54, "push ", 1, 1 );
    DECL_OPCODE( 55, "push ", 1, 1 );
    DECL_OPCODE( 56, "push ", 1, 1 );
    DECL_OPCODE( 57, "push ", 1, 1 );

    // pop reg
    DECL_OPCODE( 58, "pop ", 1, 1 );
    DECL_OPCODE( 59, "pop ", 1, 1 );
    DECL_OPCODE( 5A, "pop ", 1, 1 );
    DECL_OPCODE( 5B, "pop ", 1, 1 );
    DECL_OPCODE( 5C, "pop ", 1, 1 );
    DECL_OPCODE( 5D, "pop ", 1, 1 );
    DECL_OPCODE( 5E, "pop ", 1, 1 );
    DECL_OPCODE( 5F, "pop ", 1, 1 );

    // jb/jc imm8
    DECL_OPCODE( 72, "jb ", 2, 2 );

    // jz/je imm8
    DECL_OPCODE( 74, "jz ", 2, 2 );

    // jnz/jne imm8
    DECL_OPCODE( 75, "jnz ", 2, 2 );

    // add/sub/cmp reg8, imm8
    DECL_OPCODE( 80, "", 3, 3 );

    // add/sub/cmp reg, imm16/32
    DECL_OPCODE( 81, "add/sub/cmp ", 6, 4 );

    // mov reg, reg or mov [reg+disp], reg
    DECL_OPCODE( 89, "mov ", 2, 2 );

    // mov reg8, reg8 or mov reg8, [reg]
    DECL_OPCODE( 8A, "mov ", 2, 2 );

    // mov reg, [reg+disp]
    DECL_OPCODE( 8B, "mov ", 3, 3 );

    // mov sreg, reg
    DECL_OPCODE( 8E, "mov ", 2, 2 );

    // stosb
    DECL_OPCODE( AA, "stosb ", 1, 1 );

    // lodsb
    DECL_OPCODE( AC, "lodsb ", 1, 1 );

    // mov reg8, imm8
    DECL_OPCODE( B0, "mov ", 2, 2 );
    DECL_OPCODE( B1, "mov ", 2, 2 );
    DECL_OPCODE( B2, "mov ", 2, 2 );
    DECL_OPCODE( B3, "mov ", 2, 2 );
    DECL_OPCODE( B4, "mov ", 2, 2 );
    DECL_OPCODE( B5, "mov ", 2, 2 );
    DECL_OPCODE( B6, "mov ", 2, 2 );
    DECL_OPCODE( B7, "mov ", 2, 2 );

    // mov reg, immediate
    DECL_OPCODE( B8, "mov ", 5, 3 );
    DECL_OPCODE( B9, "mov ", 5, 3 );
    DECL_OPCODE( BA, "mov ", 5, 3 );
    DECL_OPCODE( BB, "mov ", 5, 3 );
    DECL_OPCODE( BC, "mov ", 5, 3 );
    DECL_OPCODE( BD, "mov ", 5, 3 );
    DECL_OPCODE( BE, "mov ", 5, 3 );
    DECL_OPCODE( BF, "mov ", 5, 3 );

    // rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
    DECL_OPCODE( D1, "rol/ror/rcl/rcr/shl/shr/shl/sar ", 2, 2 );

    // ret
    DECL_OPCODE( C3, "ret ", 0, 0 );

    // mov [reg], imm8
    DECL_OPCODE( C6, "mov ", 3, 3 );

    // mov [reg+disp], imm16/32
    DECL_OPCODE( C7, "mov ", 7, 5 );

    // int imm8
    DECL_OPCODE( CD, "int ", 2, 2 );

    // call imm16/32
    DECL_OPCODE( E8, "call ", 5, 3 );

    // jmp imm16/32
    DECL_OPCODE( E9, "jmp ", 5, 3 );

    // clc
    DECL_OPCODE( F8, "clc ", 1, 1 );

    // stc
    DECL_OPCODE( F9, "stc ", 1, 1 );

    // inc/dec reg8
    DECL_OPCODE( FE, "inc/dec ", 2, 2 );
}