.globl _start
_start:
mov $0x80, %al
and 3(%esp), %al
cmp $0, %al
pop %eax
je zero
pushl $-1
push %eax
jmp end
zero:
pushl $0
push %eax
end:
