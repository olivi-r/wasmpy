[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
popcnt eax, eax
popcnt ecx, ecx
add eax, ecx
push ax
push strict word 0
push strict word 0
push strict word 0
push strict word 4
