[BITS 32]
fld qword [esp]
fld qword 8[esp]
add esp, 16
fcomip
fstp dword [esp]
jbe true
push 0
jmp end
true:
push 1
end:
