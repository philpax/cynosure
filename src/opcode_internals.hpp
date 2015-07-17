#ifndef _OPCODE_INTERNALS_H_
#define _OPCODE_INTERNALS_H_

#include "VMState.hpp"

struct Opcode;

typedef void (*OpcodeFunc)(VMState*, Opcode&);
#define MAKE_OPCODE(opc) void OP##_##opc(VMState* state, Opcode& op)
#define DECL_OPCODE(opc, name, offset32, offset16)                                                 \
    opcodes[opc] = Opcode(OP##_##opc, opc, name, offset32, offset16)

struct Opcode
{
    OpcodeFunc func;        // Function pointer to the opcode handler
    std::string name;       // Opcode name
    uint8_t opcode = 0;     // Opcode associated with this
    uint8_t offset32 = 0;   // 32 bit translation
    uint8_t offset16 = 0;   // 16 bit translation
    uint8_t insnOffset = 0; // state->eip translation: for when this particular opcode needs a different
                            // translation

    uint8_t GetOffset(VMState* state)
    {
        if (state->CR0.protectedMode)
            return offset32 + insnOffset;
        else
            return offset16 + insnOffset;
    }

    uint8_t GetFinalOffset(VMState* state)
    {
        uint8_t offset = GetOffset(state);
        insnOffset = 0;
        return offset;
    }

    Opcode()
    {
    }

    Opcode(OpcodeFunc func, uint8_t opcode, std::string name, uint8_t offset32, uint8_t offset16) :
        func(func),
        opcode(opcode),
        name(name),
        offset32(offset32),
        offset16(offset16)
    {
    }
};

#endif