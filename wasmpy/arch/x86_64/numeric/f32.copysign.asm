[BITS 64]
mov al, 3[rsp]
and al, 0x80
add rsp, 4
mov cl, 3[rsp]
and cl, 0x80
cmp cl, al
je end
xor byte 3[rsp], 0x80
end:
