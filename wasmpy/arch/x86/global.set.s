.globl _start
_start:
pop %ax
cmp $4, %ax
je v64
pop %ax
movw %ax, (0xff0000ff)
pop %ax
movw %ax, (0xff000000)
jmp end
v64:
pop %ax
movw %ax, (0xff00ffff)
pop %ax
movw %ax, (0xff00ff00)
pop %ax
movw %ax, (0xff0000ff)
pop %ax
movw %ax, (0xff000000)
end:
