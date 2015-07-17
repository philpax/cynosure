#pragma once

#include "register.hpp"
#include <vector>
#include <fstream>

struct VMState
{
  private:
    void InitializeHDD();
    void LoadBootsector();

  public:
    VMState(std::string floppyDisk, std::string logFilename, uint32_t memorySize);
    ~VMState();

    void LogRegisters();

    union
    {
        Register32 general[9];
        struct
        {
            Register32 eax; // EAX - Accumulator Register
            Register32 ecx; // ECX - Counter Register
            Register32 edx; // EDX - Data Register

            Register32 ebx; // EBX - Base Register
            Register32 esp; // ESP - Stack Pointer
            Register32 ebp; // EBP - Base Stack Pointer

            Register32 esi; // ESI - Source Index
            Register32 edi; // EDI - Destination Index
            Register32 eip; // EIP - Instruction Pointer
        };
    };
    
    RegisterCR0 CR0;
    RegisterEFLAGS eflags;

    union
    {
        Register16 segment[6];
        struct
        {
            Register16 es; // ES - Extra Segment
            Register16 cs; // CS - Code Segment
            Register16 ss; // SS - Stack Segment
            Register16 ds; // DS - Data Segment
            Register16 fs; // FS - Extra Extra Segment
            Register16 gs; // GS - Extra Extra Extra Segment
        };
    };

    uint32_t memorySize;
    uint8_t* memory;

    bool running;
    std::ofstream log;

    std::fstream floppy;
    std::vector<std::fstream*> HDD;
};