[BITS 32]
mov eax, 64
mov ebx, 64
add eax, ebx
jmp $

times 510-($-$$) db 0
dw 0AA55h