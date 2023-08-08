.globl _start
_start:
movb 3(%rsp), %al
andb $0x80, %al
addq $4, %rsp
movb 3(%rsp), %cl
andb $0x80, %cl
cmpb %al, %cl
je end
xorb $0x80, 3(%rsp)
end:
