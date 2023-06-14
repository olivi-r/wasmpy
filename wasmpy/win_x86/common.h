#include <stdint.h>
#include <Windows.h>

#define V32 0x6A, 2
#define V64 0x6A, 4

#define PUSH(x) 0x6A, x

#define MOV_AX 0x66, 0xB8
#define PUSH_AX 0x66, 0x50

#define POP_AX 0x66, 0x58
#define POP_CX 0x66, 0x59
#define SHL_EAX 0xC1, 0xE0, 16
#define SHL_ECX 0xC1, 0xE1, 16
#define SHR_EAX 0xC1, 0xE8, 16

#define POP_EAX POP_AX, POP_AX, SHL_EAX, POP_AX
#define POP_ECX POP_CX, POP_CX, SHL_ECX, POP_CX
