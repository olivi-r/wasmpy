[BITS 32]
fld qword [esp]
sub esp, 2
fstcw [esp]
mov ax, [esp]
and ah, 0b11110011
or ax, 0x400
push ax
fldcw [esp]
frndint
fldcw 2[esp]
pop eax
fstp qword [esp]
