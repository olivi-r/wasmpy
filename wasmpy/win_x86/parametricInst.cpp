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
