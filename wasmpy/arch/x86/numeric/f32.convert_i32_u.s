testb $0x80, 3(%esp)
fildl (%esp)
je end
movl $0x4f800000, (%esp)
fadds (%esp)
end:
fstps (%esp)
