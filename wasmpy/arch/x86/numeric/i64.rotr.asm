[BITS 32]
pop ecx
pop eax
pop eax
pop edx
xor ebx, ebx
shrd ebx, eax, cl
shrd eax, edx, cl
shr edx, cl
or edx, ebx
test cl, 32
jne switch
push edx
switch:
push eax
je end
push edx
end:
