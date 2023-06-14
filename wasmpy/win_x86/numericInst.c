#include "numericInst.h"

// constants pushed to stack in 16 bit blocks with length appended

uint8_t *i32_const(uint32_t n)
{
    // mov ax, n[15:0]
    // push ax
    // mov ax, n[31:16]
    // push ax
    // push 2
    uint16_t hi = n >> 16;
    uint16_t lo = n & 0xFFFF;
    uint8_t *buf = malloc(14);
    memcpy(buf, (uint8_t[]){MOV_AX, lo & 255, lo >> 8, PUSH_AX, MOV_AX, hi & 255, hi >> 8, PUSH_AX, V32}, 14);
    return buf;
}

uint8_t *i64_const(uint64_t n)
{
    // mov ax, n[15:0]
    // push ax
    // mov ax, n[31:16]
    // push ax
    // mov ax, n[47:32]
    // push ax
    // mov ax, n[63:48]
    // push ax
    // push 4
    uint16_t hh = n >> 48;
    uint16_t hl = n >> 32 & 0xFFFF;
    uint16_t lh = n >> 16 & 0xFFFF;
    uint16_t ll = n & 0xFFFF;
    uint8_t *buf = malloc(26);
    memcpy(buf, (uint8_t[]){MOV_AX, ll & 255, ll >> 8, PUSH_AX, MOV_AX, lh & 255, lh >> 8, PUSH_AX, MOV_AX, hl & 255, hl >> 8, PUSH_AX, MOV_AX, hh & 255, hh >> 8, PUSH_AX, V64}, 26);
    return buf;
}

uint8_t *f32_const(float z)
{
    uint32_t n;
    memcpy(&n, &z, 4);
    return i32_const(n);
}

uint8_t *f64_const(double z)
{
    uint64_t n;
    memcpy(&n, &z, 8);
    return i64_const(n);
}

uint8_t *i32_eqz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // cmp eax, 0
    // je true
    // push 0
    // jmp end
    // true:
    // push 1
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(24);
    memcpy(buf, (uint8_t[]){POP_V32A, 0x83, 0xF8, 0, 0x74, 4, PUSH(0), 0xEB, 2, PUSH(1), PUSH(0), V32}, 24);
    return buf;
}

uint8_t *i32_eq()
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
    // push 0
    // jmp end
    // true:
    // push 1
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(32);
    memcpy(buf, (uint8_t[]){POP_V32A, POP_V32B, 0x39, 0xC8, 0x74, 4, PUSH(0), 0xEB, 2, PUSH(1), PUSH(0), V32}, 32);
    return buf;
}

uint8_t *i32_ne()
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
    // push 1
    // jmp end
    // false:
    // push 0
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(32);
    memcpy(buf, (uint8_t[]){POP_V32A, POP_V32B, 0x39, 0xC8, 0x74, 4, PUSH(1), 0xEB, 2, PUSH(0), PUSH(0), V32}, 32);
    return buf;
}

uint8_t *i64_eqz()
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
    // push 1
    // jmp end
    // false:
    // push 0
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(35);
    memcpy(buf, (uint8_t[]){POP_V64A, 0x39, 0xC8, 0x75, 9, 0x83, 0xF8, 0, 0x75, 4, PUSH(1), 0xEB, 2, PUSH(0), PUSH(0), V32}, 35);
    return buf;
}

uint8_t *i64_eq()
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
    // push 1
    // jmp end
    // false:
    // push 0
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(50);
    memcpy(buf, (uint8_t[]){POP_V64A, POP_V64B, 0x39, 0xD0, 0x75, 8, 0x39, 0xD9, 0x75, 4, PUSH(1), 0xEB, 2, PUSH(0), PUSH(0), V32}, 50);
    return buf;
}

uint8_t *i64_ne()
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
    // push 1
    // jmp end
    // equal:
    // cmp ecx, edx
    // jne true
    // push 0
    // end:
    // push 0
    // push 2
    uint8_t *buf = malloc(50);
    memcpy(buf, (uint8_t[]){POP_V64A, POP_V64B, 0x39, 0xD0, 0x74, 4, PUSH(1), 0xEB, 6, 0x39, 0xD1, 0x75, -8, PUSH(0), PUSH(0), V32}, 50);
    return buf;
}

uint8_t *i32_clz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // lzcnt eax, eax
    // push ax
    // push 0
    // push 2
    uint8_t *buf = malloc(19);
    memcpy(buf, (uint8_t[]){POP_V32A, 0xF3, 0x0F, 0xBD, 0xC0, PUSH_AX, PUSH(0), V32}, 19);
    return buf;
}

uint8_t *i32_ctz()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // tzcnt eax, eax
    // push ax
    // push 0
    // push 2
    uint8_t *buf = malloc(19);
    memcpy(buf, (uint8_t[]){POP_V32A, 0xF3, 0x0F, 0xBC, 0xC0, PUSH_AX, PUSH(0), V32}, 19);
    return buf;
}

uint8_t *i32_popcnt()
{
    // pop ax
    // pop ax
    // shl eax, 16
    // pop ax
    // popcnt eax, eax
    // push ax
    // push 0
    // push 2
    uint8_t *buf = malloc(19);
    memcpy(buf, (uint8_t[]){POP_V32A, 0xF3, 0x0F, 0xB8, 0xC0, PUSH_AX, PUSH(0), V32}, 19);
    return buf;
}

uint8_t *i32_add()
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
    // push 2
    uint8_t *buf = malloc(29);
    memcpy(buf, (uint8_t[]){POP_V32A, POP_V32B, 0x01, 0xC8, PUSH_AX, SHR_EAX, PUSH_AX, V32}, 29);
    return buf;
}

uint8_t *i32_sub()
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
    // push 2
    uint8_t *buf = malloc(29);
    memcpy(buf, (uint8_t[]){POP_V32B, POP_V32A, 0x29, 0xC8, PUSH_AX, SHR_EAX, PUSH_AX, V32}, 29);
    return buf;
}

uint8_t *i64_clz()
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
    // push 0
    // push 0
    // push 0
    // push 4
    uint8_t *buf = malloc(41);
    memcpy(buf, (uint8_t[]){POP_V64A, 0xF3, 0x0F, 0xBD, 0xC0, 0x83, 0xF8, 32, 0x75, 6, 0xF3, 0x0F, 0xBD, 0xC9, 0x01, 0xC8, PUSH_AX, PUSH(0), PUSH(0), PUSH(0), V64}, 41);
    return buf;
}

uint8_t *i64_ctz()
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
    // push 0
    // push 0
    // push 0
    // push 4
    uint8_t *buf = malloc(41);
    memcpy(buf, (uint8_t[]){POP_V64A, 0xF3, 0x0F, 0xBC, 0xC9, 0x83, 0xF9, 32, 0x75, 6, 0xF3, 0x0F, 0xBC, 0xC0, 0x01, 0xC1, PUSH_CX, PUSH(0), PUSH(0), PUSH(0), V64}, 41);
    return buf;
}

uint8_t *i64_popcnt()
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
    // push 0
    // push 0
    // push 0
    // push 4
    uint8_t *buf = malloc(36);
    memcpy(buf, (uint8_t[]){POP_V64A, 0xF3, 0x0F, 0xB8, 0xC0, 0xF3, 0x0F, 0xB8, 0xC9, 0x01, 0xC8, PUSH_AX, PUSH(0), PUSH(0), PUSH(0), PUSH(4)}, 36);
    return buf;
}
