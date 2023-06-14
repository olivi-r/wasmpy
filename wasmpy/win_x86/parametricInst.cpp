#include "parametricInst.h"

bytes drop()
{
    // pop ax
    // cmp ax, 2
    // je x32
    // pop ax
    // pop ax
    // x32:
    // pop ax
    // pop ax
    return {POP_AX, 0x66, 0x83, 0xF8, 2, 0x74, 4, POP_AX, POP_AX, POP_AX, POP_AX};
}

bytes select()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // cmp ax, 0
    // je zero
    // pop ax
    // cmp ax, 2
    // je nezerox32
    // pop ax
    // pop ax
    // nezerox32:
    // pop ax
    // pop ax
    // jmp end
    // zero:
    // pop ax
    // cmp ax, 2
    // je zerox32
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // pop dx
    // pop dx
    // pop dx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push 4
    // jmp end
    // zerox32:
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // pop cx
    // push ax
    // shr eax, 16
    // push ax
    // push 2
    // end:
    return {POP_V32A, 0x66, 0x83, 0xF8, 0, 0x74, 18, POP_AX, 0x66, 0x83, 0xF8, 2, 0x74, 4, POP_AX, POP_AX, POP_AX, POP_AX, 0xEB, 72, POP_AX, 0x66, 0x83, 0xF8, 2, 0x74, 42, POP_EAX, POP_ECX, POP_DX, POP_DX, POP_DX, POP_DX, POP_DX, PUSH_CX, SHR_ECX, PUSH_CX, PUSH_AX, SHR_EAX, PUSH_AX, V64, 0xEB, 22, POP_EAX, POP_CX, POP_CX, POP_CX, PUSH_AX, SHR_EAX, PUSH_AX, V32};
}
