#include "register.hpp"
#include "opcode_internals.hpp"
#include <iostream>
#ifdef _WIN32
    #include <conio.h>
#endif

struct diskAddressPacket
{
    uint8_t     sizePacket; // size of packet (16 bytes)
    uint8_t     padding; // always 0
    uint16_t    sectorTransferCount; // number of sectors to transfer
    uint32_t    transferBuffer; // 16 bit segment: 16 bit offset
    uint32_t    startingLBA;
    uint32_t    upperLBA; // used for upper part of 48 bit LBAs
};

MAKE_OPCODE(CD)
{
    LOG_STREAM << "0x" << (uint32_t)NEXT_INS(1);
    uint8_t driveNumber = EDX.l ^ 0x80;

    switch (NEXT_INS(1))
    {
    case 0x10: // Display characters
        switch (EAX.h)
        {
        case 0xE:
            putchar( EAX.l );
            LOG_STREAM << " (Wrote " << utils::EscapeCharacter( EAX.l ) << ")";
            break;
        };
        break;
    case 0x13: // Hard drive functions
        switch (EAX.h)
        {
        case 0x41: // Supports LBA            
            EFLAGS.carry = false;
            break;
        case 0x42:
            //diskAddressPacket *packet = (diskAddressPacket *)MEMORY( SEGMEM( DS.r, ESI.r ) );
            //packet->
            break;
        };
        break;
    case 0x16: // Read from stdin
#ifdef _WIN32
        EAX.l = _getch( );
#else
        EAX.l = getchar( ); // sorry, non-windows users
#endif
        LOG_STREAM << " (Read " << utils::EscapeCharacter( EAX.l ) << ")";
        EAX.h = 0;
        break;
    };    
}