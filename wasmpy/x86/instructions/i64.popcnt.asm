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
push word 0
push word 0
push word 0
push word 4
