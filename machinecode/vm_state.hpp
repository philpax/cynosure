#ifndef _VM_STATE_HPP_
#define _VM_STATE_HPP_
#include "register.hpp"
#include <vector>
#include <fstream>

struct vm_state
{
    vm_state( std::string filename, uint32_t memorySize, std::vector<std::string> hardDrives );
    ~vm_state( );

    void LogRegisters( );

    reg32 general[9];
    regCR0 CR0;
    regEFLAGS eflags;

    reg16 segment[6];

    uint32_t memorySize;
    uint8_t *memory;

    bool running;
    std::ofstream log;
    std::vector<std::fstream *> HDD;
};

#endif