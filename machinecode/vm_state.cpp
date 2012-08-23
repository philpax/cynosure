#include "vm_state.hpp"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

vm_state::vm_state( std::string floppyDisk, std::string logFilename, uint32_t memorySize ) : memorySize(memorySize)
{
    log.open( logFilename );
    if (!log.is_open())
        exit(-1); // can't do much if you can't log properly

    log << "Cynosure x86 Emulator - compiled " << __DATE__ << " at " << __TIME__ << std::endl;

    floppy.open( floppyDisk, std::ios::in | std::ios::binary );
    if (!floppy.is_open())
        exit(-2); // floppy dead? what are we supposed to boot from?

    memory = new uint8_t[memorySize];
    log << "[INIT] Initialized " << memorySize << " KB of memory" << std::endl;
    
    InitializeHDD( );
    LoadBootsector( );
}

vm_state::~vm_state( )
{
    log << "Shutting down VM!" << std::endl;
    BOOST_FOREACH( std::fstream *hd, HDD )
    {
        hd->close();
        delete hd;
    }

    HDD.clear();
    delete[] memory;

    floppy.close();
    log.close();
}

void vm_state::InitializeHDD( )
{
    boost::filesystem::path currentDirectory( boost::filesystem::current_path() );
    for (boost::filesystem::directory_iterator it(currentDirectory); it != boost::filesystem::directory_iterator(); ++it )
    {
        if ( it->path().filename().string().find(".fs") != std::string::npos )
        {
            HDD.push_back( new std::fstream( it->path().filename().string(), std::fstream::in | std::fstream::out | std::fstream::binary ) );
            log << "[INIT] Loaded HDD: " << it->path().filename().string() << std::endl;
        }
    }
}

void vm_state::LoadBootsector( )
{
    uint8_t *inst = new uint8_t[512];
    floppy.seekp( std::ios::beg );
    floppy.read( reinterpret_cast<char *>(inst), 512 );

    if ( *(uint16_t *)(inst+510) != 0xAA55 )
    {
        log << "[ERROR] Bootsector identifier not found, exiting" << std::endl;
        exit(-3);
    }

    segment[ 1 ].r = 0;
    general[ 8 ].r = 0x7C00;
    memcpy( memory + SEGMEM( segment[ 1 ].r, general[ 8 ].r ), inst, 512 );   
    log << "[INIT] Succesfully loaded bootsector to 0x0000:0x7C00" << std::endl;

    delete inst;
}

void vm_state::LogRegisters( )
{
    vm_state *state = this;

    for (int i = 0; i < 9; i++)
       LOG_STREAM << R_Gn(i) << ": " << PRINT_VALUE( R_G(i).r ) << " ";
    LOG_STREAM << std::endl;
}
