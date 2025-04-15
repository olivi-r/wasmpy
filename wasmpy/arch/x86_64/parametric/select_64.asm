[BITS 64]
cmp long [rsp], 0
je zero
add rsp, 12
jmp end
zero:
add rsp, 4
pop rax
mov [rsp], rax
end:
