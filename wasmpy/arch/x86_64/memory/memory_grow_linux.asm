[BITS 64]
mov rbx, 0xFF00000000000000
mov rdi, 0xFF000000000000FF
mov esi, [rsp]
call rbx
mov [rsp], eax
