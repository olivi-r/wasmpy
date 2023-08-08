.globl _start
_start:
flds (%rsp)
subq $2, %rsp
fstcw (%rsp)
movw (%rsp), %ax
andb $0b11110011, %ah
pushw %ax
fldcw (%rsp)
frndint
fldcw 2(%rsp)
addq $4, %rsp
fstps (%rsp)
