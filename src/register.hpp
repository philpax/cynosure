#pragma once

#include <iostream>
#include <iomanip>
#include "utils.hpp"

struct VMState;

typedef uint16_t Register16;
typedef uint32_t Register32;

template <typename Ret, typename T>
inline Ret& GetOffsetValue(T& value, int offset)
{
    return *reinterpret_cast<Ret*>(reinterpret_cast<uint8_t*>(&value) + offset);
}

template <typename T>
inline uint8_t& GetLowerByte(T& value)
{
    return GetOffsetValue<uint8_t>(value, 0);
}

template <typename T>
inline uint8_t& GetUpperByte(T& value)
{
    return GetOffsetValue<uint8_t>(value, 1);
}

uint8_t& GetRegister8(VMState* state, uint8_t index);

struct RegisterEFLAGS
{
    bool carry : 1;
    bool _pad1 : 1;
    bool parity : 1;
    bool _pad2 : 1;
    bool adjust : 1;
    bool _pad3 : 1;
    bool zero : 1;
    bool sign : 1;
    bool trap : 1;
    bool interrupt : 1;
    bool direction : 1;
    bool overflow : 1;
    uint8_t ioPrivilege : 2;
    bool nestedTask : 1;
    bool _pad4 : 1;
    bool resume : 1;
    bool virtual8086 : 1;
    bool alignment : 1;
    bool virtualInterrupt : 1;
    bool virtualInterruptPending : 1;
    bool ID : 1;
    uint32_t _pad5 : 11;
};

struct RegisterCR0
{
    bool protectedMode : 1;
    bool monitorCoProcessor : 1;
    bool emulation : 1;
    bool taskSwitched : 1;
    bool extensionType : 1;
    bool numericError : 1;
    uint16_t _pad1 : 10;
    bool writeProtect : 1;
    bool _pad2 : 1;
    bool alignmentMask : 1;
    uint16_t _pad3 : 10;
    bool notWriteThrough : 1;
    bool cacheDisable : 1;
    bool paging : 1;
};

union ModRM
{
    struct
    {
        uint8_t reg1 : 3;
        uint8_t reg2 : 3;
        uint8_t mod : 2;
    };

    uint8_t byte;

    ModRM(uint8_t _byte) : byte(_byte)
    {
    }
};

#define MEMORY(x) (state->memory[(x)])
#define SEGMEM(seg, offset) (((seg)*16) + (offset))
#define NEXT_INS(i) (MEMORY(state->eip + (i)))
#define CURR_INS (NEXT_INS(0))

#define Log (state->log)

// Converts the next four 8-bit integers (plus an offset) to one 32-bit integer
#define ARG_32B(offset) state->Read<int32_t>(state->eip + offset)
// Converts the next two 8-bit integers (plus an offset) to one 16-bit integer
#define ARG_16B(offset) state->Read<int16_t>(state->eip + offset)
// Converts an EIP offset to an integer, and outputs a value based on current CPU state
#define ARG(offset) state->ReadImmediate(state->eip + offset)

// Sometimes, the opcodes use a custom register operand. This returns the correct address for them.
uint8_t RegisterCombinationToMemoryAddress(VMState* state, uint8_t value); 