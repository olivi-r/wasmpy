[BITS 32]
pop ecx
pop eax
pop eax
pop edx
xor ebx, ebx
shld ebx, edx, cl
shld edx, eax, cl
shl eax, cl
or eax, ebx
test cl, 32
jne switch
push edx
switch:
push eax
je end
push edx
end:
