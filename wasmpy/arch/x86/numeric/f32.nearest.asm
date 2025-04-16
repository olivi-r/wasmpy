[BITS 32]
fld dword [esp]
sub esp, 2
fstcw [esp]
mov ax, [esp]
and ah, 0b11110011
push ax
fldcw [esp]
frndint
fldcw 2[esp]
pop eax
fstp dword [esp]
