[BITS 64]
mov rax, 0xFF00000000000000
mov eax, [rax]
sub rsp, 4
mov [rsp], eax
