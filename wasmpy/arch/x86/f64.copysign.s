.globl _start
_start:
movb 7(%esp), %al
andb $0x80, %al
addl $8, %esp
movb 7(%esp), %cl
andb $0x80, %cl
cmpb %al, %cl
je end
xorb $0x80, 7(%esp)
end:
