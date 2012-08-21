#include "register.hpp"

vm_state::vm_state( std::string filename, uint32_t memorySize ) : memorySize(memorySize)
{
    log.open( filename );
    memory = new uint8_t[memorySize];
}

vm_state::~vm_state( )
{
    log << "Shutting down VM and freeing memory" << std::endl;
    delete[] memory;
    log.close();
}

void vm_state::LogRegisters( )
{
    vm_state *state = this;

    for (int i = 0; i < 9; i++)
       LOG_STREAM << R_Gn(i) << ": 0x" << std::uppercase << std::setw(8) << std::right << std::setfill('0') << R_G(i).r << " ";
    LOG_STREAM << std::endl;
}

uint8_t &GetLHRegister( vm_state *state, uint8_t index )
{
    switch (index)
    {
    case 0:
        return EAX.l;
    case 1:
        return ECX.l;
    case 2:
        return EDX.l;
    case 3:
        return EBX.l;
    case 4:
        return EAX.h;
    case 5:
        return ECX.h;
    case 6:
        return EDX.h;
    case 7:
        return EBX.h;
    };

    return EAX.l;
}

uint8_t RegisterCombinationToMemoryAddress( vm_state *state, uint8_t value )
{
    switch (value)
    {
    case 0:
        return EBX.r + ESI.r;
    case 1:
        return EBX.r + EDI.r;
    case 2:
        return EBP.r + ESI.r;
    case 3:
        return EBP.r + EDI.r;
    case 4:
        return ESI.r;
    case 5:
        return EDI.r;
    case 6:
        return EBP.r;
    case 7:
        return EBX.r;
    };

    return 0;
}