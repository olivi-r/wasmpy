fldl (%esp)
fldl 8(%esp)
add $16, %esp
fcomip
fstps (%esp)
jae true
pushl $0
jmp end
true:
pushl $1
end:
