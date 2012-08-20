# Cynosure
## A x86 emulator
Because hey, why not?
Should be cross-platform, but not guaranteed - you will have to create your own Makefiles (there are also some Windows-only features)

### Implemented:
* Real mode
* 1MB of memory
* If an infinite loop (jmp $) occurs, execution will automatically end
* Extensive logging: writes everything to a debug file, including opcode and arguments, and write memory access
* The following opcodes:
    * add reg, reg
    * add reg, [reg+disp]
    * or reg8, reg8
    * xor reg8, reg8
    * cmp reg8, reg8
    * cmp al, imm8
    * cmp eax, imm32
    * inc reg
    * dec reg
    * push reg
    * pop reg
    * jb/jc imm8
    * jz/je imm8
    * jnz/jne imm8
    * add/sub/cmp reg8, imm8
    * add/sub/cmp reg, imm16/32
    * mov reg, reg or mov [reg+disp], reg
    * mov reg8, reg8 or mov reg8, [reg]
    * mov reg, [reg+disp]
    * mov sreg, reg
    * stosb
    * lodsb
    * mov reg8, imm8
    * mov reg, immediate
    * rol/ror/rcl/rcr/shl/shr/shl/sar reg, 1
    * ret
    * mov [reg], imm8
    * mov [reg+disp], imm16/32
    * int imm8
    * call imm16/32
    * jmp imm16/32
    * clc
    * stc
    * inc/dec reg8

### Includes:
* Visual Studio 2012 solution
* Four test examples (and their NASM source code (unless stated otherwise, only works in protected mode)):
    * call_test.asm: Tests calling functionality; sets up two variables, adds them (in a separate function), leaves the result in EAX and sets EBX to 64. Ends in infinite loop.
    * loop_test.asm: Moves 1 into EAX, and then loops until EAX matches 32 (through shifting left). After finishing, 0xDEADBEEF is moved into ECX. Ends in infinite loop.
    * rebuild_test.asm: Used to test functionality after restarting project, does a simple addition. Ends in infinite loop.
    * bootloader.asm: [Real mode assembly I](http://wiki.osdev.org/Real_mode_assembly_I) verbatim. Used as an example of "real world" code. Does not end, will continually receive input
* A bochsrc.txt configured to run bootloader.bin, so that comparisons against a tried and tested emulator can be made
* compile_asm.bat, a simple batch file that compiles all assembly files with nasm