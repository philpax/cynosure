#pragma once

#include "register.hpp"
#include <vector>
#include <fstream>

struct VMState
{
  public:
    VMState(std::string floppyDisk, std::string logFilename, uint32_t memorySize);
    ~VMState();

    char const* GetByteRegisterName(uint8_t index);
    char const* GetRegisterName(uint8_t index);
    char const* GetSegmentName(uint8_t index);
    char const* GetRegisterCombinationName(uint8_t index);
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

    // Memory functions
    void Write(uint32_t location, void const* data, size_t count);

    template <typename T>
    void Write(uint32_t location, T const& value)
    {
        Write(location, &value, sizeof(value));
    }

    template <typename T>
    T& Read(uint32_t location)
    {
        return *reinterpret_cast<T*>(&memory[location]);
    }

    template <typename T>
    T& Read(uint32_t segment, uint32_t offset)
    {
        return *reinterpret_cast<T*>(&memory[segment * 16 + offset]);
    }

    int32_t ReadImmediate(uint32_t location);
    uint8_t& ReadIPRelative(int32_t offset);

    void Push(uint32_t value);
    uint32_t Pop();

  private:
    void InitializeHDD();
    void LoadBootsector();
};