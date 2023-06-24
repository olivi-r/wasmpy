[bits 32]
pop ax
pop ax
push ax
and ax, 0x8000
cmp ax, 0
je zero
push word 0xffff
push word 0xffff
jmp end
zero:
push word 0
push word 0
end:
push word 4
