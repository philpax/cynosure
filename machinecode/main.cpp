#include "vm_state.hpp"
#include "register.hpp"
#include "opcodes.hpp"

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #undef CR0
#endif

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

int main( int argc, char **argv )
{
#ifdef _WIN32
    SetConsoleTitle( "Cynosure" );
#else
    struct termios termattr;
    tcgetattr(0, &termattr);
    termattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &termattr);
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

    boost::filesystem::path currentDirectory( boost::filesystem::current_path() );
    std::vector<std::string> hardDrives;
    for (boost::filesystem::directory_iterator it(currentDirectory); it != boost::filesystem::directory_iterator(); ++it )
    {
        if ( it->path().filename().string().find(".fs") != std::string::npos )
            hardDrives.push_back( it->path().filename().string() );
    }

    #ifdef _WIN32
        SetConsoleTitle( ( std::string("Cynosure - running ").append( std::string(argv[1]) ).c_str() ) );
    #endif

    vm_state *state = new vm_state("debug.log", 1024 * 1024);//, hardDrives);

    if (!LOG_STREAM.is_open())
        return -1;

    LOG_STREAM << "Cynosure x86 Emulator - compiled " << __DATE__ << " at " << __TIME__ << std::endl;
    
    if (instLength < 512)
    {
        LOG_STREAM << "[ERROR] Bootsector not large enough, exiting" << std::endl;
        return -1;
    }

    if ( *(uint16_t *)(inst+510) != 0xAA55 )
    {
        LOG_STREAM << "[ERROR] Bootsector identifier not found, exiting" << std::endl;
        return -1;
    }
   
    LOG_STREAM << "[INIT] Initialized " << state->memorySize << " KB of memory" << std::endl;
    LOG_STREAM << std::hex << std::uppercase << std::setfill('0');

    memcpy( state->memory + 0x7C00, inst, instLength );   
    LOG_STREAM << "[INIT] Copied bootsector to 0x7C00" << std::endl;
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
        state->LogRegisters( );
#else
        LOG_STREAM << std::endl;
#endif
    }
    
    delete state;
    return 0;
} 