.globl _start
_start:
pop %rax
movq %rax, (0xff00000000000000)
movq $0xff000000000000ff, %rax
movq %rbp, %rsp
pop %rbp
ret
