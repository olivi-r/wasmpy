.globl _start
_start:
flds (%esp)
sub $2, %esp
fstcw (%esp)
mov (%esp), %ax
and $0b11110011, %ah
or $0x800, %ax
push %ax
fldcw (%esp)
frndint
fldcw 2(%esp)
pop %eax
fstps (%esp)
