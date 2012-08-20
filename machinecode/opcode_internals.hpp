#ifndef _OPCODE_INTERNALS_H_
#define _OPCODE_INTERNALS_H_

struct opcode;

typedef void (*opcodeFunc)(vm_state *, opcode &);
#define MAKE_OPCODE( opc ) void OP ## _ ## opc( vm_state *state, opcode &op )
#define DECL_OPCODE( opc, name, offset32, offset16 ) opcodes[0x##opc] = opcode(OP ## _ ## opc, 0x##opc, name, offset32, offset16)

struct opcode
{
    opcodeFunc func; // Function pointer to the opcode handler
    std::string name; // Opcode name
    uint8_t opc; // Opcode associated with this
    uint8_t offset32; // 32 bit translation
    uint8_t offset16; // 16 bit translation
    uint8_t ins_offset; // EIP translation: for when this particular opcode needs a different translation
    uint8_t GetOffset( vm_state *state ) { if (state->CR0.protectedMode) { return offset32 + ins_offset; } else { return offset16 + ins_offset; } }
    uint8_t GetFinalOffset( vm_state *state ) { uint8_t offset = GetOffset(state); ins_offset = 0; return offset; }
    opcode( ) : func(0), opc(0), offset32(0), offset16(0), ins_offset(0) { }
    opcode( opcodeFunc _func, uint8_t _opcode, std::string _name, uint8_t _offset32, uint8_t _offset16 ) : func(_func), opc(_opcode), name(_name), offset32(_offset32), offset16(_offset16), ins_offset(0) {};
};

#endif