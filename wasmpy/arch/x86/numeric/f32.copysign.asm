[BITS 32]
mov al, 3[esp]
and al, 0x80
pop ecx
mov cl, 3[esp]
and cl, 0x80
cmp cl, al
je end
xor byte 3[esp], 0x80
end:
