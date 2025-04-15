[BITS 64]
mov eax, 255[rbp]
sub rsp, 4
mov [rsp], eax
