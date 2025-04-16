[BITS 32]
fld dword [esp]
pop eax
fadd dword [esp]
fstp dword [esp]
