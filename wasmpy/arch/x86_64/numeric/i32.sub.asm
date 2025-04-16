[BITS 64]
mov eax, [rsp]
add rsp, 4
sub [rsp], eax
