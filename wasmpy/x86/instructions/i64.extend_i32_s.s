.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
push ax
and ax, 0x8000
cmp ax, 0
je zero
pushw 0xffff
pushw 0xffff
jmp end
zero:
pushw 0
pushw 0
end:
pushw 4
