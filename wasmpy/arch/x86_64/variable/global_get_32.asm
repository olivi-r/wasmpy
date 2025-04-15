[BITS 64]
mov eax, [qword 0xff00000000000000]
sub rsp, 4
mov [rsp], eax
