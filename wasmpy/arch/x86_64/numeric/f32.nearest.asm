[BITS 64]
fld dword [rsp]
sub rsp, 2
fstcw [rsp]
mov ax, [rsp]
and ah, 0b11110011
push ax
fldcw [rsp]
frndint
fldcw 2[rsp]
add rsp, 4
fstp dword [rsp]
