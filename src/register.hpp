#pragma once

#include <iostream>
#include <iomanip>
#include "utils.hpp"

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

    ModRM(uint8_t _byte) :
        byte(_byte)
    {
    }
};

struct VMState;

#define ES (state->segment[0]) // ES - Extra Segment
#define CS (state->segment[1]) // CS - Code Segment
#define SS (state->segment[2]) // SS - Stack Segment
#define DS (state->segment[3]) // DS - Data Segment
#define FS (state->segment[4]) // FS - Extra Extra Segment
#define GS (state->segment[5]) // GS - Extra Extra Extra Segment

static const char* R_Gn16[9] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "ip"};
static const char* R_Gn32[9] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eip"};

static const char* R_Sn[6] = {"es", "cs", "ss", "ds", "fs", "gs"};
static const char* R_LHn[9] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

static const char* R_RCn32[8] = {"ebx+esi", "ebx+edi", "ebp+esi", "ebp+edi",
                                 "esi",     "edi",     "ebp",     "ebx"};
static const char* R_RCn16[8] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};

#define R_Gn(x) (state->CR0.protectedMode ? R_Gn32[(x)] : R_Gn16[(x)])
#define R_RCn(x) (state->CR0.protectedMode ? R_RCn32[(x)] : R_RCn16[(x)])

#define MEMORY(x) (state->memory[(x)])
#define SEGMEM(seg, offset) (((seg)*16) + (offset))
#define NEXT_INS(i) (MEMORY(state->eip + (i)))
#define CURR_INS (NEXT_INS(0))

#define LOG_STREAM (state->log)

#define R_G(x) (state->general[x]) // General register easy access
#define R_S(x) (state->segment[x]) // Segment register easy access

// Converts the next four 8-bit integers (plus an offset) to one 32-bit integer
#define ARG_32B(offset)                                                                            \
    CONV_8B_TO_32B(NEXT_INS(offset), NEXT_INS(offset + 1), NEXT_INS(offset + 2),                   \
                   NEXT_INS(offset + 3))
// Converts the next two 8-bit integers (plus an offset) to one 16-bit integer
#define ARG_16B(offset) CONV_8B_TO_16B(NEXT_INS(offset), NEXT_INS(offset + 1))
// Converts an EIP offset to an integer, and outputs a value based on current CPU state
#define ARG(offset) (state->CR0.protectedMode ? (int32_t)ARG_32B(offset) : (int16_t)ARG_16B(offset))

// Converts the next four 8-bit integers in memory (with a given memory location) to one 32-bit
// integer
#define ARG_32B_M(mem)                                                                             \
    CONV_8B_TO_32B(MEMORY(mem), MEMORY(mem + 1), MEMORY(mem + 2), MEMORY(mem + 3))
// Converts the next two 8-bit integers in memory (with a given memory location) to one 16-bit
// integer
#define ARG_16B_M(mem) CONV_8B_TO_16B(MEMORY(mem), MEMORY(mem + 1))
// Converts a memory location to an integer, and outputs a value based on current CPU state
#define ARG_M(mem) (state->CR0.protectedMode ? ARG_32B_M(mem) : ARG_16B_M(mem))

uint8_t& GetLHRegister(VMState* state, uint8_t index);                     // 8-bit registers
uint8_t RegisterCombinationToMemoryAddress(VMState* state, uint8_t value); // Sometimes, the
                                                                            // opcodes use a custom
                                                                            // register operand.
                                                                            // This returns the
                                                                            // correct value