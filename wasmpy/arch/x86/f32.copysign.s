.globl _start
_start:
mov 3(%esp), %al
and $0x80, %al
pop %ecx
mov 3(%esp), %cl
and $0x80, %cl
cmp %al, %cl
je end
xorb $0x80, 3(%esp)
end:
