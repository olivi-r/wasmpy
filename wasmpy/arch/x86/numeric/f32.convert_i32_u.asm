[BITS 32]
test byte 3[esp], 0x80
fild dword [esp]
je end
mov dword [esp], 0x4f800000
fadd dword [esp]
end:
fstp dword [esp]
