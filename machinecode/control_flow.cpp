#include "control_flow.hpp"
#include "memory.hpp"

// cmp reg8, reg8
MAKE_OPCODE(38) 
{ 
    modRM mod(NEXT_INS(1));
    switch (mod.mod)
    {
    case 0:
        arithmetic::Sub( state, MEMORY( SEGMEM( DS.r, GetLHRegister( state, mod.reg1 ) ) ), GetLHRegister( state, mod.reg2 ) );
        break;
    case 3:
        arithmetic::Sub( state, GetLHRegister( state, mod.reg1 ), GetLHRegister( state, mod.reg2 ) ); 
        break;
    };    
}

// cmp al, imm8
MAKE_OPCODE(3C) { LOG_STREAM << "al " << PRINT_VALUE((int32_t)NEXT_INS(1)); arithmetic::Sub( state, EAX.l, NEXT_INS(1) ); }

// cmp eax, imm32
MAKE_OPCODE(3D) { LOG_STREAM << R_Gn(0) << " " << PRINT_VALUE( ARG(1) ); arithmetic::Sub( state, EAX.r, ARG(1) ); }

// jc/jb imm8
MAKE_OPCODE(72) { LOG_STREAM << PRINT_VALUE( (int32_t)NEXT_INS(1) ); if (EFLAGS.carry) { EIP.r += (int32_t)NEXT_INS(1); } }

// jz/je imm8
MAKE_OPCODE(74) { LOG_STREAM << PRINT_VALUE( (int32_t)NEXT_INS(1) ); if (EFLAGS.zero) { EIP.r += (int32_t)NEXT_INS(1); } }

// jnz/jne imm8
MAKE_OPCODE(75) { LOG_STREAM << PRINT_VALUE( (int32_t)NEXT_INS(1) ); if (!EFLAGS.zero) { EIP.r += (int32_t)NEXT_INS(1); } }

// nop
MAKE_OPCODE(90) { }

// ret
MAKE_OPCODE(C3) { EIP = memory::Pop( state ); }

// call imm32
MAKE_OPCODE(E8) { LOG_STREAM << PRINT_VALUE( (uint32_t)ARG(1) ); memory::Push( state, EIP.r + op.GetOffset( state ) ); EIP.r += ARG(1); }

// jmp imm32
MAKE_OPCODE(E9) { if (ARG(1) == -5 || ARG(1) == -3) { state->running = false; } LOG_STREAM << PRINT_VALUE( (uint32_t)ARG(1) ); EIP.r += ARG(1); }