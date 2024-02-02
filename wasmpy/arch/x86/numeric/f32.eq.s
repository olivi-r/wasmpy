pop %eax
pop %ecx
cmp %eax, %ecx
je true
pushl $0
jmp end
true:
pushl $1
end:
