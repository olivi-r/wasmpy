[bits 32]
pop ax
pop ax
push ax
and ax, 0x8000
cmp ax, 0
je zero
push strict word 0xffff
push strict word 0xffff
jmp end
zero:
push strict word 0
push strict word 0
end:
push strict word 4
