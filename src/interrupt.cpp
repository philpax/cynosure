#include "register.hpp"
#include "opcode_internals.hpp"
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#endif

struct diskAddressPacket
{
    uint8_t sizePacket;           // Size of packet (16 bytes)
    uint8_t _padding;             // Always 0
    uint16_t sectorTransferCount; // Number of sectors to transfer

    uint16_t offsetAddr;  // The 16-bit offset, used with...
    uint16_t segmentAddr; // The 16-bit segment

    uint32_t startingLBA; // First part of the 48-bit LBAs,
    uint32_t upperLBA;    // Used for upper part of 48 bit LBAs
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
            putchar(EAX.l);
            LOG_STREAM << " (Wrote " << utils::EscapeCharacter(EAX.l) << ")";
            break;
        };
        break;
    case 0x13: // Hard drive functions
        switch (EAX.h)
        {
        case 0x02: // Read floppy
        {
            // Sectors: 18 | Cylinders: 80 | Heads: 2 | 512 bytes per sector
            uint8_t sectorCount = EAX.l;
            uint8_t cylinder = ECX.h;
            uint8_t sector = ECX.l;
            uint8_t head = EDX.h;

            uint8_t* addr = (uint8_t*)(state->memory + SEGMEM(ES.r, EBX.r));
            uint32_t LBA =
                ((cylinder * 2) + head) * 18 + sector - 1; // LBA = ((C * HPC) + H) * SPT + S - 1
            uint32_t readAmount = sectorCount * 512;

            uint8_t* tempStorage = new uint8_t[readAmount];
            state->floppy.seekg(LBA * 512);
            state->floppy.read((char*)tempStorage, readAmount);
            memcpy(addr, tempStorage, readAmount);
            delete tempStorage;

            EFLAGS.carry = false;
            EAX.h = 0;
        }
        break;
        case 0x03: // Write floppy
            break;
        case 0x41: // Supports LBA
            EFLAGS.carry = false;
            break;
        case 0x42: // Read hard drive, extended LBA
        {
            diskAddressPacket* packet = (diskAddressPacket*)(state->memory + SEGMEM(DS.r, ESI.r));
            LOG_STREAM << std::endl << "INT 0x13 packet:" << std::endl;
            LOG_STREAM << " Size: " << PRINT_VALUE((uint32_t)packet->sizePacket) << std::endl;
            LOG_STREAM << " Sectors to transfer: " << PRINT_VALUE(packet->sectorTransferCount)
                       << std::endl;
            LOG_STREAM << " Memory address: " << PRINT_VALUE(packet->segmentAddr) << ":"
                       << PRINT_VALUE(packet->offsetAddr) << std::endl;
            LOG_STREAM << " LBA (32 bits out of 48): " << PRINT_VALUE(packet->startingLBA)
                       << std::endl;
            // Lazy hack: assume a sector is 512 bytes
            uint32_t readAmount = packet->sectorTransferCount * 512;

            uint8_t* addr =
                (uint8_t*)(state->memory + SEGMEM(packet->segmentAddr, packet->offsetAddr));
            uint8_t* tempStorage = new uint8_t[readAmount];
            state->HDD[driveNumber]->seekg((packet->startingLBA - 1) * 512);
            state->HDD[driveNumber]->read((char*)tempStorage, readAmount);
            memcpy(addr, tempStorage, readAmount);
            delete tempStorage;

            EFLAGS.carry = false;
            EAX.h = 0;
            break;
        }
        };
        break;
    case 0x16: // Read from stdin
#ifdef _WIN32
        EAX.l = _getch();
#else
        EAX.l = getchar();
#endif
        // Remap line feed to carriage return
        if (EAX.l == '\n')
            EAX.l = '\r';

        LOG_STREAM << " (Read " << utils::EscapeCharacter(EAX.l) << ")";
        EAX.h = 0;
        break;
    };
}