# Cynosure
## A x86 emulator
Because hey, why not?

Should be cross-platform, but not guaranteed - you will have to create your own Makefiles (there are also some Windows-only features). The project was recently restarted, so there may be remnants of old code used for re-implementation.

### Implemented:
* Real mode
* 1MB of memory
* If an infinite loop (jmp $) occurs, execution will automatically end
* Extensive logging: writes everything to a debug file, including opcode and arguments, and write memory access. Successful jump instructions are also 
* A small set of opcodes (see OPCODES.md)

### Notes:
* No protected mode switching implemented yet, so to run a PM sample, you must recompile with protected mode on (see main.cpp)

### Includes:
* Visual Studio 2012 solution
* Four test examples (and their NASM source code):
    * call_test.asm: *Protected mode* Tests calling functionality; sets up two variables, adds them (in a separate function), leaves the result in EAX and sets EBX to 64. Ends in infinite loop.
    * loop_test.asm: *Protected mode* Moves 1 into EAX, and then loops until EAX matches 32 (through shifting left). After finishing, 0xDEADBEEF is moved into ECX. Ends in infinite loop.
    * rebuild_test.asm: *Protected mode* Used to test functionality after restarting project, does a simple addition. Ends in infinite loop.
    * bootloader.asm: *Real mode* [Real mode assembly I](http://wiki.osdev.org/Real_mode_assembly_I) derivative. Used as an example of "real world" code. Use the command 'halt' to quit.
* A bochsrc.txt configured to run bootloader.bin, so that comparisons against a tried and tested emulator can be made
* compile_asm.bat, a simple batch file that compiles all assembly files with nasm