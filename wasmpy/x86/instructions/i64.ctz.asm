.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
pop cx
shl ecx, 16
pop cx
cmp eax, 0
je highzero
bsf eax, eax
add ax, 32
push ax
jmp end
highzero:
cmp ecx, 0
je lowzero
bsf ecx, ecx
push cx
jmp end
lowzero:
pushw 64
end:
pushw 0
pushw 0
pushw 0
pushw 4
