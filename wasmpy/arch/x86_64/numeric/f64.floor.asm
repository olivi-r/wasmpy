[BITS 64]
fld qword [rsp]
sub rsp, 2
fstcw [rsp]
mov ax, [rsp]
and ah, 0b11110011
or ax, 0x400
push ax
fldcw [rsp]
frndint
fldcw 2[rsp]
add rsp, 4
fstp qword [rsp]
