[BITS 32]
mov eax, 1
jmp loop

loop:
	shl eax, 1
	cmp eax, 32
	je finish
	jmp loop

finish:
	mov ecx, 0xDEADBEEF
	jmp $

times 510-($-$$) db 0
dw 0AA55h