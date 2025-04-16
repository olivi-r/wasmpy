[BITS 32]
fld dword [esp]
pop eax
fld dword [esp]
fcomip
fstp dword [esp]
jb true
mov dword [esp], 0
jmp end
true:
mov dword [esp], 1
end:
