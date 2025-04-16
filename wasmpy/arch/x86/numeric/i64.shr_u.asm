[BITS 32]
pop ecx
pop eax
pop eax
pop edx
shrd eax, edx, cl
shr edx, cl
test cl, 32
je switch
push 0
push edx
jmp end
switch:
push edx
push eax
end:
