.globl _start
_start:
mov 7(%rsp), %al
and $0x80, %al
add $8, %rsp
mov 7(%rsp), %cl
and $0x80, %cl
cmp %al, %cl
je end
xorb $0x80, 7(%rsp)
end:
