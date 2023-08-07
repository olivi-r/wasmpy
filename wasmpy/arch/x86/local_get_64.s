.globl _start
_start:
movl 255(%ebp), %eax
pushl %eax
movl 254(%ebp), %eax
pushl %eax
end:
