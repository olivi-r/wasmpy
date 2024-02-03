mov (%rsp), %eax
add $4, %rsp
mov %eax, (0xff00000000000000)
