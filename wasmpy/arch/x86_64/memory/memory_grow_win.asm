[BITS 64]
mov rbx, 0xFF00000000000000
mov rcx, 0xFF000000000000FF
mov edx, [rsp]
sub rsp, 28
call rbx
add rsp, 28
mov [rsp], eax
