#include <stdint.h>
#include <Windows.h>

#define PUSH(x) 0x6A, x

#define V32 PUSH(2)
#define V64 PUSH(4)

#define MOV_AX 0x66, 0xB8
#define PUSH_AX 0x66, 0x50
#define PUSH_CX 0x66, 0x51

#define POP_AX 0x66, 0x58
#define POP_CX 0x66, 0x59
#define POP_DX 0x66, 0x5A
#define POP_BX 0x66, 0x5B

#define SHL_EAX 0xC1, 0xE0, 16
#define SHL_ECX 0xC1, 0xE1, 16
#define SHL_EDX 0xC1, 0xE2, 16
#define SHL_EBX 0xC1, 0xE3, 16

#define SHR_EAX 0xC1, 0xE8, 16
#define SHR_ECX 0xC1, 0xE9, 16

#define POP_EAX POP_AX, SHL_EAX, POP_AX
#define POP_ECX POP_CX, SHL_ECX, POP_CX
#define POP_EDX POP_DX, SHL_EDX, POP_DX
#define POP_EBX POP_BX, SHL_EBX, POP_BX

#define POP_V32A POP_AX, POP_EAX
#define POP_V32B POP_CX, POP_ECX
#define POP_V64A POP_AX, POP_EAX, POP_ECX
#define POP_V64B POP_DX, POP_EDX, POP_EBX
