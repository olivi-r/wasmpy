.globl _start
_start:
andb $0x7f, 3(%esp)
andb $0x7f, 7(%esp)
andb $0x7f, 11(%esp)
andb $0x7f, 15(%esp)
