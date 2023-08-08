.globl _start
_start:
fldl (%rsp)
subq $2, %rsp
fstcw (%rsp)
movw (%rsp), %ax
andb $0b11110011, %ah
orw $0x400, %ax
pushw %ax
fldcw (%rsp)
frndint
fldcw 2(%rsp)
addq $4, %rsp
fstpl (%rsp)
