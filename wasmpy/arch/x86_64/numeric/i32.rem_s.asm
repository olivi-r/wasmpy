[BITS 64]
mov ecx, [rsp]
add rsp, 4
mov eax, [rsp]
cmp ecx, 0
je zero
cmp eax, 0x80000000
jne cont
cmp ecx, 0xffffffff
jne cont
mov dword [rsp], 0
jmp end
zero:
mov rax, 0xff00000000000000
leave
ret
cont:
cdq
idiv ecx
mov [rsp], edx
end:
