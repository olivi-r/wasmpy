[BITS 32]
fld dword [esp]
pop eax
fdivr dword [esp]
fst dword [esp]
