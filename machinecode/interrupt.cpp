#include "register.hpp"
#include "opcode_internals.hpp"
#include <iostream>
#ifdef _WIN32
    #include <conio.h>
#endif

MAKE_OPCODE(CD)
{
    LOG_STREAM << "0x" << (uint32_t)NEXT_INS(1);

    switch (NEXT_INS(1))
    {
    case 0x10:
        switch (EAX.h)
        {
        case 0xE:
            putchar( EAX.l );
            break;
        };
        break;
    case 0x16:
#ifdef _WIN32
        EAX.l = _getch( );
#else
        EAX.l = getchar( ); // sorry, non-windows users
#endif
        EAX.h = 0;
        break;
    };    
}