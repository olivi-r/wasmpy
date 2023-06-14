#include "parametricInst.h"

uint8_t *drop()
{
    // pop ax
    // cmp ax, 2
    // je x32
    // pop ax
    // pop ax
    // x32:
    // pop ax
    // pop ax
    uint8_t *buf = malloc(16);
    memcpy(buf, (uint8_t[]){POP_AX, 0x66, 0x83, 0xF8, 2, 0x74, 4, POP_AX, POP_AX, POP_AX, POP_AX}, 16);
    return buf;
}
