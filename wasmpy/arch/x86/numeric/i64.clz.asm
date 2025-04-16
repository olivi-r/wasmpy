[BITS 32]
pop eax
pop ecx
cmp ecx, 0
je highzero
bsr ecx, ecx
mov eax, 31
sub eax, ecx
push 0
push eax
jmp end
highzero:
cmp eax, 0
je zero
bsr eax, eax
mov ecx, 63
sub ecx, eax
push 0
push ecx
jmp end
zero:
push 0
push 64
end:
