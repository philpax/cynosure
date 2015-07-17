#include "Register.hpp"
#include "Opcodes.hpp"
#include "VMState.hpp"

#include <iostream>
#include <memory>

#ifdef _WIN32
#include <conio.h>
#endif

struct DiskAddressPacket
{
    uint8_t sizePacket;           // Size of packet (16 bytes)
private:
    uint8_t _padding;             // Always 0
public:
    uint16_t sectorTransferCount; // Number of sectors to transfer

    uint16_t offsetAddr;  // The 16-bit offset, used with...
    uint16_t segmentAddr; // The 16-bit segment

    uint32_t startingLBA; // First part of the 48-bit LBAs,
    uint32_t upperLBA;    // Used for upper part of 48 bit LBAs
};

void Int0x10(VMState* state)
{
    // Display characters
    auto mode = GetUpperByte(state->eax);

    if (mode == 0xE)
    {
        putchar(GetLowerByte(state->eax));
        Log << " (Wrote " << utils::EscapeCharacter(GetLowerByte(state->eax)) << ")";
    }
}

void Int0x13(VMState* state)
{
    // Hard drive functions
    auto mode = GetUpperByte(state->eax);

    // Read floppy
    if (mode == 0x02)
    {
        // Sectors: 18 | Cylinders: 80 | Heads: 2 | 512 bytes per sector
        uint8_t sectorCount = GetLowerByte(state->eax);
        uint8_t cylinder = GetUpperByte(state->ecx);
        uint8_t sector = GetLowerByte(state->ecx);
        uint8_t head = GetUpperByte(state->edx);

        uint8_t* addr = &state->Read<uint8_t>(state->es, state->ebx);
        // LBA = ((C * HPC) + H) * SPT + S - 1
        uint32_t lba = ((cylinder * 2) + head) * 18 + sector - 1;
        uint32_t readAmount = sectorCount * 512;

        std::unique_ptr<uint8_t[]> tempStorage(new uint8_t[readAmount]);
        state->floppy.seekg(lba * 512);
        state->floppy.read(reinterpret_cast<char*>(tempStorage.get()), readAmount);
        memcpy(addr, tempStorage.get(), readAmount);

        state->eflags.carry = false;
        GetUpperByte(state->eax) = 0;
    }
    // Write floppy
    else if (mode == 0x03)
    {
    }
    // Supports LBA
    else if (mode == 0x41)
    {
        state->eflags.carry = false;
    }
    // Read hard drive, extended LBA
    else if (mode == 0x42)
    {
        auto packet = &state->Read<DiskAddressPacket>(state->ds, state->esi);
        Log << std::endl << "INT 0x13 packet:" << std::endl;

        Log << " Size: "
            << PRINT_VALUE((uint32_t)packet->sizePacket)
            << std::endl;

        Log << " Sectors to transfer: "
            << PRINT_VALUE(packet->sectorTransferCount)
            << std::endl;

        Log << " Memory address: "
            << PRINT_VALUE(packet->segmentAddr)
            << ":"
            << PRINT_VALUE(packet->offsetAddr)
            << std::endl;

        Log << " LBA (32 bits out of 48): "
            << PRINT_VALUE(packet->startingLBA)
            << std::endl;

        // Lazy hack: assume a sector is 512 bytes
        uint32_t readAmount = packet->sectorTransferCount * 512;

        uint8_t* addr = &state->Read<uint8_t>(packet->segmentAddr, packet->offsetAddr);

        uint8_t driveNumber = GetLowerByte(state->edx) ^ 0x80;
        auto hdd = state->HDD[driveNumber];
        std::unique_ptr<uint8_t[]> tempStorage(new uint8_t[readAmount]);
        hdd->seekg((packet->startingLBA - 1) * 512);
        hdd->read(reinterpret_cast<char*>(tempStorage.get()), readAmount);
        memcpy(addr, tempStorage.get(), readAmount);

        state->eflags.carry = false;
        GetUpperByte(state->eax) = 0;
    }
}

void Int0x16(VMState* state)
{
    // Read from stdin
    uint8_t readCharacter = 0;
#ifdef _WIN32
    readCharacter = _getch();
#else
    readCharacter = getchar();
#endif
    // Remap line feed to carriage return
    if (readCharacter == '\n')
        readCharacter = '\r';

    GetLowerByte(state->eax) = readCharacter;
    GetUpperByte(state->eax) = 0;

    Log << " (Read " << utils::EscapeCharacter(readCharacter) << ")";
}

MAKE_OPCODE(0xCD)
{
    Log << "0x" << (uint32_t)state->ReadIPRelative(1);

    switch (state->ReadIPRelative(1))
    {
    case 0x10:
        Int0x10(state);
        break;
    case 0x13:
        Int0x13(state);
        break;
    case 0x16:
        Int0x16(state);
        break;
    };
}