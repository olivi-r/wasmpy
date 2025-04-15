[BITS 64]
cmp long [rsp], 0
je zero
add rsp, 8
jmp end
zero:
mov eax, 4[rsp]
add rsp, 8
mov [rsp], eax
end:
