#include "arithmetic.hpp"
#include "memory.hpp"

uint32_t arithmetic::Add( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a + b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    return value;
}

uint32_t arithmetic::Sub( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a - b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    return value;
}

uint32_t arithmetic::And( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a & b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    EFLAGS.overflow = false;
    EFLAGS.carry = false;

    return value;
}

uint32_t arithmetic::Or( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a | b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    EFLAGS.overflow = false;
    EFLAGS.carry = false;

    return value;
}

uint32_t arithmetic::Xor( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a ^ b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    EFLAGS.overflow = false;
    EFLAGS.carry = false;

    return value;
}

uint32_t arithmetic::ShiftLeft( vm_state *state, uint32_t a, uint32_t b )
{
    uint32_t value = a << b;
    if (value == 0)             { EFLAGS.zero = true; } else { EFLAGS.zero = false; }
    if ( (int32_t)value < 0 )   { EFLAGS.sign = true; } else { EFLAGS.sign = false; }

    EFLAGS.overflow = false;
    EFLAGS.carry = false;

    return value;
}

// add reg, reg
MAKE_OPCODE(01) 
{ 
    modRM mod(NEXT_INS(1)); 
    R_G(mod.reg1).r = arithmetic::Add( state, R_G(mod.reg1).r, R_G(mod.reg2).r ); 
}

// add reg, [reg+disp]
MAKE_OPCODE(03) 
{ 
    modRM mod(NEXT_INS(1)); 
    if(mod.mod == 1) 
    { 
        R_G(mod.reg2).r = arithmetic::Add( state, R_G(mod.reg2).r, MEMORY( R_G(mod.reg1).r + (int8_t)NEXT_INS(2) ) ); 
    } 
}


// or reg8, reg8
MAKE_OPCODE(08)
{
    modRM mod(NEXT_INS(1)); 
    LOG_STREAM << R_LHn[ mod.reg1 ] << ", " << R_LHn[ mod.reg2 ];
    GetLHRegister( state, mod.reg1 ) = arithmetic::Or( state, GetLHRegister( state, mod.reg1 ), GetLHRegister( state, mod.reg2 ) );
}

// xor reg8, reg8
MAKE_OPCODE(30)
{
    modRM mod(NEXT_INS(1)); 
    LOG_STREAM << R_LHn[ mod.reg1 ] << ", " << R_LHn[ mod.reg2 ];
    GetLHRegister( state, mod.reg1 ) = arithmetic::Xor( state, GetLHRegister( state, mod.reg1 ), GetLHRegister( state, mod.reg2 ) );
}

MAKE_OPCODE(40) { EAX.r = arithmetic::Add( state, EAX.r, 1 ); LOG_STREAM << "eax"; }
MAKE_OPCODE(41) { ECX.r = arithmetic::Add( state, ECX.r, 1 ); LOG_STREAM << "ecx"; }
MAKE_OPCODE(42) { EDX.r = arithmetic::Add( state, EDX.r, 1 ); LOG_STREAM << "edx"; }
MAKE_OPCODE(43) { EBX.r = arithmetic::Add( state, EBX.r, 1 ); LOG_STREAM << "ebx"; }
MAKE_OPCODE(44) { ESP.r = arithmetic::Add( state, ESP.r, 1 ); LOG_STREAM << "esp"; }
MAKE_OPCODE(45) { EBP.r = arithmetic::Add( state, EBP.r, 1 ); LOG_STREAM << "ebp"; }
MAKE_OPCODE(46) { ESI.r = arithmetic::Add( state, ESI.r, 1 ); LOG_STREAM << "esi"; }
MAKE_OPCODE(47) { EDI.r = arithmetic::Add( state, EDI.r, 1 ); LOG_STREAM << "edi"; }

MAKE_OPCODE(48) { EAX.r = arithmetic::Sub( state, EAX.r, 1 ); LOG_STREAM << "eax"; }
MAKE_OPCODE(49) { ECX.r = arithmetic::Sub( state, ECX.r, 1 ); LOG_STREAM << "ecx"; }
MAKE_OPCODE(4A) { EDX.r = arithmetic::Sub( state, EDX.r, 1 ); LOG_STREAM << "edx"; }
MAKE_OPCODE(4B) { EBX.r = arithmetic::Sub( state, EBX.r, 1 ); LOG_STREAM << "ebx"; }
MAKE_OPCODE(4C) { ESP.r = arithmetic::Sub( state, ESP.r, 1 ); LOG_STREAM << "esp"; }
MAKE_OPCODE(4D) { EBP.r = arithmetic::Sub( state, EBP.r, 1 ); LOG_STREAM << "ebp"; }
MAKE_OPCODE(4E) { ESI.r = arithmetic::Sub( state, ESI.r, 1 ); LOG_STREAM << "esi"; }
MAKE_OPCODE(4F) { EDI.r = arithmetic::Sub( state, EDI.r, 1 ); LOG_STREAM << "edi"; }

// add/sub/cmp reg8, imm8
MAKE_OPCODE(80) 
{ 
    modRM mod(NEXT_INS(1)); 

    switch (mod.reg2)
    {
    case 0:
        LOG_STREAM << "add ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ) = arithmetic::Add( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), NEXT_INS(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            GetLHRegister( state, mod.reg1 ) = arithmetic::Add( state, GetLHRegister( state, mod.reg1 ), NEXT_INS(2) );
            break;
        };
        break;
    case 5:
        LOG_STREAM << "sub ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ) = arithmetic::Sub( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), NEXT_INS(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            GetLHRegister( state, mod.reg1 ) = arithmetic::Sub( state, GetLHRegister( state, mod.reg1 ), NEXT_INS(2) );
            break;
        };
        break;
    case 7:
        LOG_STREAM << "cmp ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            arithmetic::Sub( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), NEXT_INS(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            arithmetic::Sub( state, GetLHRegister( state, mod.reg1 ), NEXT_INS(2) );
            break;
        };
        break;
    };
}

// add/sub/cmp reg, imm16/32
MAKE_OPCODE(81) 
{ 
    modRM mod(NEXT_INS(1)); 

    switch (mod.reg2)
    {
    case 0:
        LOG_STREAM << "add ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ) = arithmetic::Add( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), ARG(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            GetLHRegister( state, mod.reg1 ) = arithmetic::Add( state, GetLHRegister( state, mod.reg1 ), ARG(2) );
            break;
        };
        break;
    case 5:
        LOG_STREAM << "sub ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ) = arithmetic::Sub( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), ARG(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            GetLHRegister( state, mod.reg1 ) = arithmetic::Sub( state, GetLHRegister( state, mod.reg1 ), ARG(2) );
            break;
        };
        break;
    case 7:
        LOG_STREAM << "cmp ";
        switch (mod.mod)
        {
        case 0:
            LOG_STREAM << '[' << R_LHn[ mod.reg1 ] << ']';
            arithmetic::Sub( state, MEMORY( SEGMEM(DS.r, GetLHRegister( state, mod.reg1 )) ), ARG(2) );
            break;
        case 3:
            LOG_STREAM << R_LHn[ mod.reg1 ];
            arithmetic::Sub( state, GetLHRegister( state, mod.reg1 ), ARG(2) );
            break;
        };
        break;
    };

    LOG_STREAM << ", " << PRINT_VALUE( ARG(2) );
}

// rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
MAKE_OPCODE(D1)
{
    modRM mod(NEXT_INS(1)); 
    switch (mod.reg2)
    {
    case 4:
        R_G( mod.reg1 ).r = arithmetic::ShiftLeft( state, R_G( mod.reg1 ).r, 1 );
        break;
    };
}

// clc
MAKE_OPCODE(F8) { EFLAGS.carry = false; }

// stc
MAKE_OPCODE(F9) { EFLAGS.carry = true; }

// inc/dec cl
MAKE_OPCODE(FE) 
{  
    modRM mod(NEXT_INS(1)); 
    switch( mod.reg2 )
    {
    case 0:
        R_G( mod.reg1 ) = arithmetic::Add( state, mod.reg1, 1 );
        break;
    case 1:
        R_G( mod.reg1 ) = arithmetic::Sub( state, mod.reg1, 1 );
        break;
    };
}