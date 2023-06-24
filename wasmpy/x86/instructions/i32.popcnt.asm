[bits 32]
pop ax
pop ax
shl eax, 16
pop ax
popcnt eax, eax
push ax
push strict word 0
push strict word 2
