.globl _start
_start:
.intel_syntax noprefix
pop ax
pop ax
shl eax, 16
pop ax
cmp eax, 0
je true
bsr eax, eax
mov cx, 31
sub cx, ax
push cx
jmp end
true:
pushw 32
end:
pushw 0
pushw 2
