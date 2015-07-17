[BITS 16]
mov ax, 0x0800
mov ds, ax
mov es, ax
mov si, success
call print_string
mov ah, 0
int 0x16
jmp $

print_string:
   lodsb        ; grab a byte from SI
 
   or al, al  ; logical or AL by itself
   jz .done   ; if the result is zero, get out
 
   mov ah, 0x0E
   int 0x10      ; otherwise, print out the character!
 
   jmp print_string
 
 .done:
   ret

success db 'Successful boot!', 0x0D, 0x0A, 0

times 512-($-$$) db 0