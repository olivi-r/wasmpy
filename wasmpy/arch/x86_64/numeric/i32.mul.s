mov (%rsp), %eax
add $4, %rsp
imull (%rsp)
mov %eax, (%rsp)
