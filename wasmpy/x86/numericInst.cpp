#include "numericInst.h"

// constants pushed to stack in 16 bit blocks with length appended

bytes i32_const(uint32_t n)
{
    // mov ax, n[15:0]
    // push ax
    // mov ax, n[31:16]
    // push ax
    // push word 2
    uint16_t hi = n >> 16;
    uint16_t lo = n & 0xFFFF;
    return {MOV_AX, (uint8_t)(lo & 255), (uint8_t)(lo >> 8), PUSH_AX, MOV_AX, (uint8_t)(hi & 255), (uint8_t)(hi >> 8), PUSH_AX, V32};
}

bytes i64_const(uint64_t n)
{
    // mov ax, n[15:0]
    // push ax
    // mov ax, n[31:16]
    // push ax
    // mov ax, n[47:32]
    // push ax
    // mov ax, n[63:48]
    // push ax
    // push word 4
    uint16_t hh = n >> 48;
    uint16_t hl = n >> 32 & 0xFFFF;
    uint16_t lh = n >> 16 & 0xFFFF;
    uint16_t ll = n & 0xFFFF;
    return {MOV_AX, (uint8_t)(ll & 255), (uint8_t)(ll >> 8), PUSH_AX, MOV_AX, (uint8_t)(lh & 255), (uint8_t)(lh >> 8), PUSH_AX, MOV_AX, (uint8_t)(hl & 255), (uint8_t)(hl >> 8), PUSH_AX, MOV_AX, (uint8_t)(hh & 255), (uint8_t)(hh >> 8), PUSH_AX, V64};
}

bytes f32_const(float z)
{
    uint32_t n;
    memcpy(&n, &z, 4);
    return i32_const(n);
}

bytes f64_const(double z)
{
    uint64_t n;
    memcpy(&n, &z, 8);
    return i64_const(n);
}

bytes i32_eqz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // cmp eax, 0
    // je true
    // push word 0
    // jmp end
    // true:
    // push word 1
    // end:
    // push word 0
    // push word 2
    return {POP_V32A, 0x83, 0xF8, 0, JE(4), PUSH(0), JMP(2), PUSH(1), PUSH(0), V32};
}

bytes i32_eq()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // cmp eax, ecx
    // je true
    // push word 0
    // jmp end
    // true:
    // push word 1
    // end:
    // push word 0
    // push word 2
    return {POP_V32A, POP_V32B, 0x39, 0xC8, JE(4), PUSH(0), JMP(2), PUSH(1), PUSH(0), V32};
}

bytes i32_ne()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // cmp eax, ecx
    // je false
    // push word 1
    // jmp end
    // false:
    // push word 0
    // end:
    // push word 0
    // push word 2
    return {POP_V32A, POP_V32B, 0x39, 0xC8, JE(4), PUSH(1), JMP(2), PUSH(0), PUSH(0), V32};
}

bytes i64_eqz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // cmp eax, ecx
    // jne false
    // cmp eax, 0
    // jne false
    // push word 1
    // jmp end
    // false:
    // push word 0
    // end:
    // push word 0
    // push word 2
    return {POP_V64A, 0x39, 0xC8, JNE(9), 0x83, 0xF8, 0, JNE(4), PUSH(1), JMP(2), PUSH(0), PUSH(0), V32};
}

bytes i64_eq()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // cmp eax, edx
    // jne false
    // cmp ecx, ebx
    // jne false
    // push word 1
    // jmp end
    // false:
    // push word 0
    // end:
    // push word 0
    // push word 2
    return {POP_V64A, POP_V64B, 0x39, 0xD0, JNE(8), 0x39, 0xD9, JNE(4), PUSH(1), JMP(2), PUSH(0), PUSH(0), V32};
}

bytes i64_ne()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // cmp eax, edx
    // je equal
    // true:
    // push word 1
    // jmp end
    // equal:
    // cmp ecx, edx
    // jne true
    // push word 0
    // end:
    // push word 0
    // push word 2
    return {POP_V64A, POP_V64B, 0x39, 0xD0, JE(4), PUSH(1), JMP(6), 0x39, 0xD1, JNE(-8), PUSH(0), PUSH(0), V32};
}

bytes i32_clz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // lzcnt eax, eax
    // push ax
    // push word 0
    // push word 2
    return {POP_V32A, 0xF3, 0x0F, 0xBD, 0xC0, PUSH_AX, PUSH(0), V32};
}

bytes i32_ctz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // tzcnt eax, eax
    // push ax
    // push word 0
    // push word 2
    return {POP_V32A, 0xF3, 0x0F, 0xBC, 0xC0, PUSH_AX, PUSH(0), V32};
}

bytes i32_popcnt()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // popcnt eax, eax
    // push ax
    // push word 0
    // push word 2
    return {POP_V32A, 0xF3, 0x0F, 0xB8, 0xC0, PUSH_AX, PUSH(0), V32};
}

bytes i32_add()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // add eax, ecx
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {POP_V32A, POP_V32B, 0x01, 0xC8, PUSH_V32};
}

bytes i32_sub()
{
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // sub eax, ecx
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {POP_V32B, POP_V32A, 0x29, 0xC8, PUSH_V32};
}

bytes i32_and()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // and eax, ecx
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {POP_V32A, POP_V32B, 0x21, 0xC8, PUSH_V32};
}

bytes i32_or()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // or eax, ecx
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {POP_V32A, POP_V32B, 0x09, 0xC8, PUSH_V32};
}

bytes i32_xor()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // pop cx
    // shl ecx, 16
    // pop cx
    // xor eax, ecx
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {POP_V32A, POP_V32B, 0x31, 0xC8, PUSH_V32};
}

bytes i32_shl()
{
    // mov cx, 32
    // pop ax
    // pop ax
    // pop dx
    // idiv cx
    // pop ax
    // shl eax, 16
    // pop ax
    // mov cl, dl
    // shl eax, cl
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {0x66, 0xB9, 32, 0, POP_AX, POP_AX, POP_DX, 0x66, 0xF7, 0xF9, POP_EAX, 0x88, 0xD1, 0xD3, 0xE0, PUSH_V32};
}

bytes i32_shr_s()
{
    // mov cx, 32
    // pop ax
    // pop ax
    // pop dx
    // idiv cx
    // pop ax
    // shl eax, 16
    // pop ax
    // mov cl, dl
    // sar eax, cl
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {0x66, 0xB9, 32, 0, POP_AX, POP_AX, POP_DX, 0x66, 0xF7, 0xF9, POP_EAX, 0x88, 0xD1, 0xD3, 0xF8, PUSH_V32};
}

bytes i32_shr_u()
{
    // mov cx, 32
    // pop ax
    // pop ax
    // pop dx
    // idiv cx
    // pop ax
    // shl eax, 16
    // pop ax
    // mov cl, dl
    // shr eax, cl
    // push ax
    // shr eax, 16
    // push ax
    // push word 2
    return {0x66, 0xB9, 32, 0, POP_AX, POP_AX, POP_DX, 0x66, 0xF7, 0xF9, POP_EAX, 0x88, 0xD1, 0xD3, 0xE8, PUSH_V32};
}

bytes i64_clz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // lzcnt eax, eax
    // cmp eax, 32
    // jne false
    // lzcnt ecx, ecx
    // add eax, ecx
    // false:
    // push ax
    // push word 0
    // push word 0
    // push word 0
    // push word 4
    return {POP_V64A, 0xF3, 0x0F, 0xBD, 0xC0, 0x83, 0xF8, 32, JNE(6), 0xF3, 0x0F, 0xBD, 0xC9, 0x01, 0xC8, PUSH_AX, PUSH(0), PUSH(0), PUSH(0), V64};
}

bytes i64_ctz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // tzcnt ecx, ecx
    // cmp ecx, 32
    // jne false
    // tzcnt eax, eax
    // add ecx, eax
    // false:
    // push cx
    // push word 0
    // push word 0
    // push word 0
    // push word 4
    return {POP_V64A, 0xF3, 0x0F, 0xBC, 0xC9, 0x83, 0xF9, 32, JNE(6), 0xF3, 0x0F, 0xBC, 0xC0, 0x01, 0xC1, PUSH_CX, PUSH(0), PUSH(0), PUSH(0), V64};
}

bytes i64_popcnt()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // popcnt eax, eax
    // popcnt ecx, ecx
    // add eax, ecx
    // push ax
    // push word 0
    // push word 0
    // push word 0
    // push word 4
    return {POP_V64A, 0xF3, 0x0F, 0xB8, 0xC0, 0xF3, 0x0F, 0xB8, 0xC9, 0x01, 0xC8, PUSH_AX, PUSH(0), PUSH(0), PUSH(0), V64};
}

bytes i64_add()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // add ecx, ebx
    // adc eax, edx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push word 4
    return {POP_V64A, POP_V64B, 0x01, 0xD9, 0x11, 0xD0, PUSH_V64};
}

bytes i64_sub()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // sub ecx, ebx
    // sbb eax, edx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push word 4
    return {POP_V64A, POP_V64B, 0x29, 0xD9, 0x19, 0xD0, PUSH_V64};
}

bytes i64_and()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // and eax, edx
    // and ecx, ebx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push word 4
    return {POP_V64A, POP_V64B, 0x21, 0xD0, 0x21, 0xD9, PUSH_V64};
}

bytes i64_or()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // or eax, edx
    // or ecx, ebx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push word 4
    return {POP_V64A, POP_V64B, 0x09, 0xD0, 0x09, 0xD9, PUSH_V64};
}

bytes i64_xor()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // pop cx
    // shl ecx, 16
    // pop cx
    // pop dx
    // pop dx
    // shl edx, 16
    // pop dx
    // pop bx
    // shl ebx, 16
    // pop bx
    // xor eax, edx
    // xor ecx, ebx
    // push cx
    // shr ecx, 16
    // push cx
    // push ax
    // shr eax, 16
    // push ax
    // push word 4
    return {POP_V64A, POP_V64B, 0x31, 0xD0, 0x31, 0xD9, PUSH_V64};
}
