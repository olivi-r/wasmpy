.globl _start
_start:
flds (%esp)
subl $2, %esp
fstcw (%esp)
movw (%esp), %ax
andb $0b11110011, %ah
pushw %ax
fldcw (%esp)
frndint
fldcw 2(%esp)
addl $4, %esp
fstps (%esp)
