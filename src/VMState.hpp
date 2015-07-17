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

    reg32 general[9];
    regCR0 CR0;
    regEFLAGS eflags;

    reg16 segment[6];

    uint32_t memorySize;
    uint8_t* memory;

    bool running;
    std::ofstream log;

    std::fstream floppy;
    std::vector<std::fstream*> HDD;
};