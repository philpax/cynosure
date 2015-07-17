#include "VMState.hpp"

VMState::VMState(std::string floppyDisk, std::string logFilename, uint32_t memorySize)
    : memorySize(memorySize)
{
    log.open(logFilename);

    if (!log.is_open())
        exit(-1);

    log << "Cynosure x86 Emulator - compiled " << __DATE__ << " at " << __TIME__ << std::endl;

    floppy.open(floppyDisk, std::ios::in | std::ios::binary);

    if (!floppy.is_open())
        exit(-2);

    memory = new uint8_t[memorySize];
    log << "[INIT] Initialized " << memorySize << " KB of memory" << std::endl;

    InitializeHDD();
    LoadBootsector();
}

VMState::~VMState()
{
    log << "Shutting down VM!" << std::endl;
    HDD.clear();

    log.close();
    floppy.close();
}

void VMState::InitializeHDD()
{
    /*
boost::filesystem::path currentDirectory( boost::filesystem::current_path() );
for (boost::filesystem::directory_iterator it(currentDirectory); it !=
boost::filesystem::directory_iterator(); ++it )
{
    if ( it->path().filename().string().find(".fs") != std::string::npos )
    {
        HDD.push_back( new std::fstream( it->path().filename().string(), std::fstream::in |
std::fstream::out | std::fstream::binary ) );
        log << "[INIT] Loaded HDD: " << it->path().filename().string() << std::endl;
    }
}
    */
}

void VMState::LoadBootsector()
{
    uint8_t bootsector[512];
    floppy.seekp(std::ios::beg);
    floppy.read(reinterpret_cast<char*>(bootsector), 512);

    if (*reinterpret_cast<uint16_t*>(bootsector + 510) != 0xAA55)
    {
        log << "[ERROR] Bootsector identifier not found, exiting" << std::endl;
        exit(-3);
    }

    segment[1] = 0;
    general[8] = 0x7C00;
    memcpy(memory + SEGMEM(segment[1], general[8]), bootsector, 512);
    log << "[INIT] Succesfully loaded bootsector to 0x0000:0x7C00" << std::endl;
}

void VMState::LogRegisters()
{
    VMState* state = this;

    for (int i = 0; i < 9; i++)
        LOG_STREAM << R_Gn(i) << ": " << PRINT_VALUE(R_G(i)) << " ";
    LOG_STREAM << std::endl;
}
