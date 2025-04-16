[BITS 32]
pop ecx
pop eax
pop eax
pop edx
shld edx, eax, cl
sal eax, cl
test cl, 32
je switch
push eax
push 0
jmp end
switch:
push edx
push eax
end:
