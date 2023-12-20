.globl _start
_start:
fldl (%esp)
sub $2, %esp
fstcw (%esp)
mov (%esp), %ax
and $0b11110011, %ah
push %ax
fldcw (%esp)
frndint
fldcw 2(%esp)
pop %eax
fstpl (%esp)
