.globl _start
_start:
movb $0x80, %al
and 3(%esp), %al
cmpb $0, %al
popl %eax
je zero
pushl $-1
pushl %eax
jmp end
zero:
pushl $0
pushl %eax
end:
