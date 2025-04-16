[BITS 64]
mov al, 7[rsp]
and al, 0x80
add rsp, 8
mov cl, 7[rsp]
and cl, 0x80
cmp cl, al
je end
xor byte 7[rsp], 0x80
end:
