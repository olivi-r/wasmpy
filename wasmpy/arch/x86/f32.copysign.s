.globl _start
_start:
movb 3(%esp), %al
andb $0x80, %al
addl $4, %esp
movb 3(%esp), %cl
andb $0x80, %cl
cmpb %al, %cl
je end
xorb $0x80, 3(%esp)
end:
