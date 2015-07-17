align 4
   mov ax, 0x07C0  ; set up segments
   mov ds, ax
   mov es, ax

   mov sp, stack_top
 
   mov si, welcome
   call print_string
 
 mainloop:
   mov si, prompt
   call print_string
 
   mov di, buffer
   call get_string
 
   mov si, buffer
   cmp byte [si], 0  ; blank line?
   je mainloop       ; yes, ignore it

   mov si, buffer
   mov di, cmd_halt
   call strcmp
   jc .halt

   mov si, buffer
   mov di, cmd_boot
   call strcmp
   jc .boot   
 
   mov si,badcommand
   call print_string 
   jmp mainloop

 .boot:
   mov ax, 0x0800
   mov es, ax

   mov ah, 2
   mov al, 1
   mov ch, 0
   mov cl, 2
   mov dh, 0
   
   mov bx, 0
   mov dl, 0
   int 0x13                ; read!
   jc short .error         ; if there was an error, error and go back
   jmp word 0x0800:0x0000  ; otherwise, boot our new code

 .halt:
   jmp $

 .error:
   mov si, msg_failed
   call print_string

   jmp mainloop
 
 welcome db 'Welcome to My OS!', 0x0D, 0x0A, 0
 msg_helloworld db 'Hello OSDev World!', 0x0D, 0x0A, 0
 badcommand db 'Bad command entered.', 0x0D, 0x0A, 0
 prompt db '>', 0
 cmd_boot db 'boot', 0
 cmd_halt db 'halt', 0
 msg_failed db 'Failed to boot!', 0x0D, 0x0A, 0
 buffer times 64 db 0
 
 ; ================
 ; calls start here
 ; ================
 
 print_string:
   lodsb        ; grab a byte from SI
 
   or al, al  ; logical or AL by itself
   jz .done   ; if the result is zero, get out
 
   mov ah, 0x0E
   int 0x10      ; otherwise, print out the character!
 
   jmp print_string
 
 .done:
   ret
 
 get_string:
   xor cl, cl
 
 .loop:
   mov ah, 0
   int 0x16   ; wait for keypress
 
   cmp al, 0x08    ; backspace pressed?
   je .backspace   ; yes, handle it
 
   cmp al, 0x0D  ; enter pressed?
   je .done      ; yes, we're done
 
   cmp cl, 0x3F  ; 63 chars inputted?
   je .loop      ; yes, only let in backspace and enter
 
   mov ah, 0x0E
   int 0x10      ; print out character
 
   stosb  ; put character in buffer
   inc cl
   jmp .loop
 
 .backspace:
   cmp cl, 0   ; beginning of string?
   je .loop ; yes, ignore the key
 
   dec di
   mov byte [di], 0  ; delete character
   dec cl      ; decrement counter as well
 
   mov ah, 0x0E
   mov al, 0x08
   int 10h     ; backspace on the screen
 
   mov al, ' '
   int 10h     ; blank character out
 
   mov al, 0x08
   int 10h     ; backspace again
 
   jmp .loop   ; go to the main loop
 
 .done:
   mov al, 0   ; null terminator
   stosb
 
   mov ah, 0x0E
   mov al, 0x0D
   int 0x10
   mov al, 0x0A
   int 0x10    ; newline
 
   ret
 
 strcmp:
 .loop:
   mov al, [si]   ; grab a byte from SI
   mov bl, [di]   ; grab a byte from DI
   cmp al, bl     ; are they equal?
   jne .notequal  ; nope, we're done.
 
   cmp al, 0  ; are both bytes (they were equal before) null?
   je .done   ; yes, we're done.
 
   inc di     ; increment DI
   inc si     ; increment SI
   jmp .loop  ; loop!
 
 .notequal:
   clc  ; not equal, clear the carry flag
   ret
 
 .done:  
   stc  ; equal, set the carry flag
   ret
 
times 510-($-$$) db 0
dw 0AA55h ; some BIOSes require this signature

stack_bottom:
times 16384 db 0
stack_top: