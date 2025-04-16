[BITS 64]
mov eax, 4[rsp]
cmp dword [rsp], 0
jne continue
mov rax, 0xff00000000000000
leave
ret
continue:
xor edx, edx
div dword [rsp]
add rsp, 4
mov [rsp], edx
