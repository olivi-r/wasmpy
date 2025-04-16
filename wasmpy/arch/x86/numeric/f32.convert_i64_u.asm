[BITS 32]
test byte 7[esp], 0x80
fild qword [esp]
je end
mov dword [esp], 0
mov dword 4[esp], 0x43f00000
fadd qword [esp]
end:
pop eax
fstp dword [esp]
