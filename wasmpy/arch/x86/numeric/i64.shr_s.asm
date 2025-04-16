[BITS 32]
pop ecx
pop eax
pop eax
pop edx
shrd eax, edx, cl
sar edx, cl
test cl, 32
je switch
push edx
sar dword [esp], 31
push edx
jmp end
switch:
push edx
push eax
end:
