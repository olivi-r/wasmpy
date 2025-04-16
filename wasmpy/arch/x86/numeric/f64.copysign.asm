[BITS 32]
mov al, 7[esp]
and al, 0x80
add esp, 8
mov cl, 7[esp]
and cl, 0x80
cmp cl, al
je end
xor byte 7[esp], 0x80
end:
