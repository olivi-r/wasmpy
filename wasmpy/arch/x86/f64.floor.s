.globl _start
_start:
fldl (%esp)
subl $2, %esp
fstcw (%esp)
movw (%esp), %ax
andb $0b11110011, %ah
orw $0x400, %ax 
pushw %ax
fldcw (%esp)
frndint
fldcw 2(%esp)
addl $4, %esp
fstpl (%esp)
