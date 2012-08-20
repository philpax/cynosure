[BITS 32]
call main
jmp finish

add:
	push ebp
	mov ebp, esp ;create new stackframe
	sub esp, 4 ;allocate space for temp variable
	mov eax, [ebp+12] ;copy second variable to eax
	add eax, [ebp+8] ;add second and first variable
	mov [ebp-4], eax ;move result into allocated space
	mov esp, ebp ;release stack frame
	pop ebp
	ret ;go back to caller

main:
	push ebp
	mov ebp, esp ;create new stackframe
	sub esp, 8 ;create space for 2 variables
	mov dword [esp+4], 25
	mov dword [esp], 10
	call add
	mov [ebp-4], eax 
	mov esp, ebp
	pop ebp
	ret

finish:
	mov ebx, 64
	jmp $