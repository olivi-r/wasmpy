.globl _start
_start:
movl 127(%rbp), %eax
subq $4, %rsp
movl %eax, (%rsp)
end:
