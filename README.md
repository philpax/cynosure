# Cynosure
## A x86 interpreter
Basic interpretation of x86-16/x86-32.

### Implemented:
* Real mode
* 1MB of memory
* If an infinite loop (jmp $) occurs, execution will automatically end
* Extensive logging: writes everything to a debug file, including opcode and arguments, and write memory access.
* A small set of opcodes

### Notes:
* No protected mode switching implemented yet, so to run a PM sample, you must recompile with protected mode on (see main.cpp)
