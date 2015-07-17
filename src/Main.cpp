#include "VMState.hpp"
#include "Register.hpp"
#include "Opcodes.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#undef CR0
#endif

#include <memory>

int main(int argc, char** argv)
{
#ifdef _WIN32
    SetConsoleTitle("Cynosure");
#else
    struct termios termattr;
    tcgetattr(0, &termattr);
    termattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &termattr);
#endif

    std::unique_ptr<VMState> state;
    try
    {
        state.reset(new VMState("floppy_1_44.img", "debug.log", 1024 * 1024));
    }
    catch (std::exception const& e)
    {
        std::cerr << "[INIT] Failed to initialize: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    state->CR0.protectedMode = false;
    state->CR0.emulation = true;

    state->eflags.direction = false;

    Opcode opcodes[256];
    Opcode::FillTable(opcodes);

    state->running = true;
    while (state->running)
    {
        Opcode opcode = opcodes[state->ReadIPRelative(0)];

        Log << "0x" << std::setw(2) << std::hex << std::uppercase
            << (uint32_t)opcode.opcode << ": ";

        opcode.func(state.get(), opcode);

        Log << std::endl;
    }

    return 0;
}