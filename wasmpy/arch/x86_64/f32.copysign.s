.globl _start
_start:
mov 3(%rsp), %al
and $0x80, %al
add $4, %rsp
mov 3(%rsp), %cl
and $0x80, %cl
cmp %al, %cl
je end
xorb $0x80, 3(%rsp)
end:
