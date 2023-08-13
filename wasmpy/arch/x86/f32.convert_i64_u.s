.globl _start
_start:
testb $0x80, 7(%esp)
fildq (%esp)
je end
movl $0, (%esp)
movl $0x43f00000, 4(%esp)
faddl (%esp)
end:
pop %eax
fstps (%esp)
