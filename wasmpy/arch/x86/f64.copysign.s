.globl _start
_start:
mov 7(%esp), %al
and $0x80, %al
add $8, %esp
mov 7(%esp), %cl
and $0x80, %cl
cmp %al, %cl
je end
xorb $0x80, 7(%esp)
end:
