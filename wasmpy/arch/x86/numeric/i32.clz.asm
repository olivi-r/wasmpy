[BITS 32]
pop eax
cmp eax, 0
je zero
bsr eax, eax
mov ecx, 31
sub ecx, eax
push ecx
jmp end
zero:
push 32
end:
