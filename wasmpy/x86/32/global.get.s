.globl _start
_start:
movb [0xffffffff], %al
cmp $4, %al
je v64
movw [0xff000000], %ax
push %ax
movw [0xff0000ff], %ax
push %ax
pushw $2
jmp end
v64:
movw [0xff000000], %ax
push %ax
movw [0xff0000ff], %ax
push %ax
movw [0xff00ff00], %ax
push %ax
movw [0xff00ffff], %ax
push %ax
pushw $4
end:
