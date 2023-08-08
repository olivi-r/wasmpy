.globl _start
_start:
movb 7(%rsp), %al
andb $0x80, %al
addq $8, %rsp
movb 7(%rsp), %cl
andb $0x80, %cl
cmpb %al, %cl
je end
xorb $0x80, 7(%rsp)
end:
