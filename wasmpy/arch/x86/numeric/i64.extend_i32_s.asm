[BITS 32]
mov al, 0x80
and al, 3[esp]
cmp al, 0
pop eax
je zero
push -1
push eax
jmp end
zero:
push 0
push eax
end:
