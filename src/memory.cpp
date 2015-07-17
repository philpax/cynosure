#include "memory.hpp"

// Helper function
void memory::Conv32BTo8B( uint32_t value, uint8_t &a, uint8_t &b, uint8_t &c, uint8_t &d )
{
    a = (uint8_t)value;
    b = (uint8_t)(value >> 8);
    c = (uint8_t)(value >> 16);
    d = (uint8_t)(value >> 24);
}

void memory::Conv16BTo8B( uint16_t value, uint8_t &a, uint8_t &b )
{
    a = (uint8_t)value;
    b = (uint8_t)(value >> 8);
}

void memory::ConvTo8B( vm_state *state, uint32_t value, uint8_t &a, uint8_t &b, uint8_t &c, uint8_t &d )
{
    if (state->CR0.protectedMode)
        memory::Conv32BTo8B( value, a, b, c, d );
    else
        memory::Conv16BTo8B( (uint16_t)value, a, b );
}

void memory::WriteValueToMemory( vm_state *state, uint32_t location, uint32_t value )
{
    uint8_t a, b, c, d;
    ConvTo8B( state, value, a, b, c, d );

    state->memory[ESP.r]     = a;
    state->memory[ESP.r + 1] = b;

    if (state->CR0.protectedMode)
    {
        state->memory[ESP.r + 2] = c;
        state->memory[ESP.r + 3] = d;
    }

    LOG_STREAM << "[MEMORY] Wrote " << value << " to " << location;
}

// Stack instructions
void memory::Push( vm_state *state, uint32_t value )
{
    ESP.r -= (state->CR0.protectedMode ? 4 : 2);
    LOG_STREAM << std::endl;
    WriteValueToMemory( state, ESP.r, value );    

    LOG_STREAM << std::endl << "[STACK] Pushed: " << value << " to " << ESP.r;
}

reg32 memory::Pop( vm_state *state )
{
    reg32 value;
    value.r = ( ARG_M( ESP.r ) );
    LOG_STREAM << std::endl << "[STACK] Popped: " << value.r << " from " << ESP.r;
    ESP.r += (state->CR0.protectedMode ? 4 : 2);
    return value;
}

// push reg
MAKE_OPCODE(50) { memory::Push( state, EAX.r ); }
MAKE_OPCODE(51) { memory::Push( state, ECX.r ); }
MAKE_OPCODE(52) { memory::Push( state, EDX.r ); }
MAKE_OPCODE(53) { memory::Push( state, EBX.r ); }
MAKE_OPCODE(54) { memory::Push( state, ESP.r ); }
MAKE_OPCODE(55) { memory::Push( state, EBP.r ); }
MAKE_OPCODE(56) { memory::Push( state, ESI.r ); }
MAKE_OPCODE(57) { memory::Push( state, EDI.r ); }

// pop reg
MAKE_OPCODE(58) { EAX = memory::Pop( state ); }
MAKE_OPCODE(59) { ECX = memory::Pop( state ); }
MAKE_OPCODE(5A) { EDX = memory::Pop( state ); }
MAKE_OPCODE(5B) { EBX = memory::Pop( state ); }
MAKE_OPCODE(5C) { ESP = memory::Pop( state ); }
MAKE_OPCODE(5D) { EBP = memory::Pop( state ); }
MAKE_OPCODE(5E) { ESI = memory::Pop( state ); }
MAKE_OPCODE(5F) { EDI = memory::Pop( state ); }

// mov reg, reg OR mov [reg+disp], reg
MAKE_OPCODE(89) 
{ 
    modRM mod(NEXT_INS(1)); 

    switch(mod.mod)
    {
    case 1:
        LOG_STREAM << "[" << R_Gn(mod.reg1) << '+' << (int32_t)NEXT_INS(2) << "], " << R_Gn(mod.reg2);
        memory::WriteValueToMemory( state, SEGMEM( DS.r, R_G(mod.reg1).r + (int8_t)NEXT_INS(2) ), R_G(mod.reg2).r );
        op.ins_offset++;
        break;
    case 3:
        LOG_STREAM << R_Gn(mod.reg1) << ", " << R_Gn(mod.reg2);
        R_G(mod.reg1) = R_G(mod.reg2); 
        break;
    };
}

// mov reg8, reg8 or mov reg8, [reg]
MAKE_OPCODE(8A) 
{ 
    modRM mod(NEXT_INS(1)); 

    switch(mod.mod)
    {
    case 0:
        LOG_STREAM << R_LHn[ mod.reg2 ] << ", [" << R_RCn( mod.reg1 ) << "]";
        GetLHRegister( state, mod.reg2 ) = MEMORY( SEGMEM( DS.r, RegisterCombinationToMemoryAddress( state, mod.reg1 ) ) );
        break;
    case 3:
        LOG_STREAM << R_LHn[ mod.reg1 ] << ", " << R_LHn[ mod.reg2 ];
        GetLHRegister( state, mod.reg1 ) = GetLHRegister( state, mod.reg2 );
        break;
    };
}

// mov reg, [reg+disp] (note: reg1 and reg2 are swapped in the modr/m byte for some reason. wtf, intel?)
MAKE_OPCODE(8B) { modRM mod(NEXT_INS(1)); if(mod.mod == 1) { R_G(mod.reg2).r = MEMORY( SEGMEM( DS.r, R_G(mod.reg1).r + (int8_t)NEXT_INS(2) ) ); } }

// mov sreg, reg
MAKE_OPCODE(8E) { modRM mod(NEXT_INS(1)); if(mod.mod == 3) { LOG_STREAM << R_Sn[mod.reg2] << ", " << R_Gn(mod.reg1); R_S(mod.reg2).r = R_G(mod.reg1).r; } }

// mov reg8, imm8
MAKE_OPCODE(B0) { LOG_STREAM << "al, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EAX.l = NEXT_INS(1); }
MAKE_OPCODE(B1) { LOG_STREAM << "cl, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); ECX.l = NEXT_INS(1); }
MAKE_OPCODE(B2) { LOG_STREAM << "dl, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EDX.l = NEXT_INS(1); }
MAKE_OPCODE(B3) { LOG_STREAM << "bl, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EBX.l = NEXT_INS(1); }
MAKE_OPCODE(B4) { LOG_STREAM << "ah, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EAX.h = NEXT_INS(1); }
MAKE_OPCODE(B5) { LOG_STREAM << "ch, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); ECX.h = NEXT_INS(1); }
MAKE_OPCODE(B6) { LOG_STREAM << "dh, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EDX.h = NEXT_INS(1); }
MAKE_OPCODE(B7) { LOG_STREAM << "bh, " << PRINT_VALUE( (uint32_t)NEXT_INS(1) ); EBX.h = NEXT_INS(1); }

// mov reg, immediate
MAKE_OPCODE(B8) { LOG_STREAM << R_Gn(0) << ", " << PRINT_VALUE( ARG( 1 ) ); EAX.r = ARG( 1 ); }
MAKE_OPCODE(B9) { LOG_STREAM << R_Gn(1) << ", " << PRINT_VALUE( ARG( 1 ) ); ECX.r = ARG( 1 ); }
MAKE_OPCODE(BA) { LOG_STREAM << R_Gn(2) << ", " << PRINT_VALUE( ARG( 1 ) ); EDX.r = ARG( 1 ); }
MAKE_OPCODE(BB) { LOG_STREAM << R_Gn(3) << ", " << PRINT_VALUE( ARG( 1 ) ); EBX.r = ARG( 1 ); }
MAKE_OPCODE(BC) { LOG_STREAM << R_Gn(4) << ", " << PRINT_VALUE( ARG( 1 ) ); ESP.r = ARG( 1 ); }
MAKE_OPCODE(BD) { LOG_STREAM << R_Gn(5) << ", " << PRINT_VALUE( ARG( 1 ) ); EBP.r = ARG( 1 ); }
MAKE_OPCODE(BE) { LOG_STREAM << R_Gn(6) << ", " << PRINT_VALUE( ARG( 1 ) ); ESI.r = ARG( 1 ); }
MAKE_OPCODE(BF) { LOG_STREAM << R_Gn(7) << ", " << PRINT_VALUE( ARG( 1 ) ); EDI.r = ARG( 1 ); }

// mov [reg], imm8
MAKE_OPCODE(C6)
{
    modRM mod(NEXT_INS(1));

    switch (mod.mod)
    {
    case 0:
        LOG_STREAM << "[" << R_G( mod.reg1 ).r << "], " << PRINT_VALUE( (uint32_t)NEXT_INS(2) ) << std::endl;
        memory::WriteValueToMemory( state, R_G( mod.reg1 ).r, NEXT_INS(2) );
        break;
    case 3:
        LOG_STREAM << R_G( mod.reg1 ).r << ", " << PRINT_VALUE( (uint32_t)NEXT_INS(2) );
        R_G( mod.reg1 ).r = NEXT_INS(2);
        break;
    };    
}

// mov [reg+disp], imm16/32
MAKE_OPCODE(C7)
{
    modRM mod(NEXT_INS(1));
    modRM sib(NEXT_INS(2)); // this is a bit of a hack, but then x86 is a bit of a hack architecture

    uint8_t displacement = 0;
    if (mod.mod == 1)
    {
        displacement = NEXT_INS(3);
        op.ins_offset++;
    }

    uint32_t memAddress = R_G(sib.reg1).r + (int32_t)displacement;
    memory::WriteValueToMemory( state, memAddress, ARG(op.GetOffset( state ) - (state->CR0.protectedMode ? 4 : 2)) );
}