# The following opcodes have been implemented:
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