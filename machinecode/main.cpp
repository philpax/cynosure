#include "register.hpp"
#include "opcodes.hpp"
#include <cstring>
#ifdef _WIN32
    #include <Windows.h>
#endif

void logRegisters( vm_state *state )
{
    for (int i = 0; i < 9; i++)
       LOG_STREAM << "0x" << std::uppercase << std::setw(8) << std::right << std::setfill('0') << R_G(i).r << " ";
    LOG_STREAM << std::endl;
}

int main( int argc, char **argv )
{
#ifdef _WIN32
    SetConsoleTitle( "Cynosure" );
#endif

    if (argc < 2)
    {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return -1;
    }

    std::ifstream file;
    file.open( argv[1], std::ios::in | std::ios::binary | std::ios::ate );

    if (!file.is_open())
        return -1;

    uint32_t instLength = (uint32_t)file.tellg();
    uint8_t *inst = new uint8_t[instLength];
    file.seekg( 0, std::ios::beg );
    file.read( reinterpret_cast<char *>(inst), instLength );
    file.close();

    #ifdef _WIN32
        SetConsoleTitle( ( std::string("Cynosure - running ").append( std::string(argv[1]) ).c_str() ) );
    #endif

    vm_state *state = new vm_state();
    std::ofstream _log("debug.log");
    state->log = &_log;

    if (!LOG_STREAM.is_open())
        return -1;
    
    if (instLength < 512)
    {
        LOG_STREAM << "Bootsector not large enough, exiting" << std::endl;
        return -1;
    }

    if ( *(uint16_t *)(inst+510) != 0xAA55 )
    {
        LOG_STREAM << "Bootsector identifier not found, exiting" << std::endl;
        return -1;
    }
    
    state->memorySize = 1024 * 1024; // 1024 bytes in a kilobyte, 1024 kilobytes in a megabyte
    state->memory = new uint8_t[state->memorySize];
    LOG_STREAM << "Initialized " << state->memorySize << " KB of memory" << std::endl;

    LOG_STREAM << std::hex << std::uppercase << std::setfill('0');

    memcpy( state->memory + 0x7C00, inst, instLength );   
    LOG_STREAM << "Copied bootsector to 0x7C00" << std::endl;
    delete[] inst;

    EBP.r = 0x00001000;
    ESP = EBP;
    EIP.r = 0x7C00;

    state->CR0.protectedMode    = false;
    state->CR0.emulation        = true;

    EFLAGS.direction = false;

    opcode opcodes[256];
    opcodesGenerate( opcodes );
    
    state->running = true;
    while (state->running)
    {
        opcode currOpcode = opcodes[ CURR_INS ];

        LOG_STREAM << "0x" << std::setw(2) << (uint32_t)currOpcode.opc << ": " << currOpcode.name.c_str();

        currOpcode.func( state, currOpcode );
        EIP.r += currOpcode.GetFinalOffset( state );

#ifdef LOG_REGISTERS
        logRegisters( state );
#endif

        LOG_STREAM << std::endl;
    }
    std::cout << "Execution ended" << std::endl;

    delete[] state->memory;
    delete state;

    std::cin.get();
    return 0;
}