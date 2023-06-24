[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
popcnt eax, eax
push ax
push word 0
push word 2
