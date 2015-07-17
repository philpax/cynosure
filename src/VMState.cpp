#include "VMState.hpp"
#include <stdexcept>

VMState::VMState(std::string floppyDisk, std::string logFilename, uint32_t memorySize)
    : memorySize(memorySize)
{
    log.open(logFilename);

    if (!log.is_open())
        throw std::runtime_error("Failed to open log file");

    log << "Cynosure x86 Emulator - compiled " << __DATE__ << " at " << __TIME__ << std::endl;

    floppy.open(floppyDisk, std::ios::in | std::ios::binary);

    if (!floppy.is_open())
        throw std::runtime_error("Failed to open floppy");

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

char const* VMState::GetByteRegisterName(uint8_t index)
{
    static char const* names8[9] =
        {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

    return names8[index];
}

char const* VMState::GetRegisterName(uint8_t index)
{
    static char const* names16[9] =
        {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "ip"};
    static char const* names32[9] =
        {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eip"};
    static char const* names8[9] =
        {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

    if (CR0.protectedMode)
        return names32[index];
    else
        return names16[index];
}

char const* VMState::GetSegmentName(uint8_t index)
{
    static char const* names[6] = {"es", "cs", "ss", "ds", "fs", "gs"};

    return names[index];
}

char const* VMState::GetRegisterCombinationName(uint8_t index)
{
    static char const* names32[8] =
        {"ebx+esi", "ebx+edi", "ebp+esi", "ebp+edi", "esi", "edi", "ebp", "ebx"};
    static char const* names16[8] =
        {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};

    if (CR0.protectedMode)
        return names32[index];
    else
        return names16[index];
}

void VMState::LogRegisters()
{
    auto state = this;

    for (int i = 0; i < 9; i++)
        log << GetRegisterName(i) << ": " << PRINT_VALUE(general[i]) << " ";

    log << std::endl;
}

// Helper function
void VMState::Write(uint32_t location, void const* data, size_t count)
{
    memcpy(&memory[location], data, count);
    log << "[MEMORY] Wrote " << count << " bytes to " << location;
}

int32_t VMState::ReadImmediate(uint32_t location)
{
    if (CR0.protectedMode)
        return Read<int32_t>(location);
    else
        return Read<int16_t>(location);
}

// Stack instructions
void VMState::Push(uint32_t value)
{
    esp -= (CR0.protectedMode ? 4 : 2);
    log << std::endl;
    Write(esp, value);

    log << std::endl << "[STACK] Pushed: " << value << " to " << esp;
}

uint32_t VMState::Pop()
{
    uint32_t value = ReadImmediate(esp);
    log << std::endl << "[STACK] Popped: " << value << " from " << esp;
    esp += (CR0.protectedMode ? 4 : 2);
    return value;
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
        throw std::runtime_error("Failed to find bootsector identifier");

    segment[1] = 0;
    general[8] = 0x7C00;

    void* destination = reinterpret_cast<void*>(&Read<uint8_t>(segment[1], general[8]));
    memcpy(destination, bootsector, 512);
    log << "[INIT] Succesfully loaded bootsector to 0x0000:0x7C00" << std::endl;
}