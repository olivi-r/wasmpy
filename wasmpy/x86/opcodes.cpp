#include "opcodes.h"

bytes concat(bytes v0, std::vector<bytes> vn)
{
    for (size_t i = 0; i < vn.size(); i++)
    {
        v0.insert(v0.end(), vn.at(i).begin(), vn.at(i).end());
    }

    return v0;
}

bytes decodeFunc(bytes buf)
{
    int localidx;
    std::vector<bytes> insts = {};
    for (size_t i = 0; i < buf.size(); i++)
    {
        uint8_t opcode = buf.at(i);
        switch (opcode)
        {
        // control instructions
        case 0x01: // nop
            break;

        // parametric instructions
        case 0x1A: // drop
            // pop ax
            // cmp ax, 2
            // je x32
            // pop ax
            // pop ax
            // x32:
            // pop ax
            // pop ax
            insts.push_back({POP_AX, 0x66, 0x83, 0xF8, 2, 0x74, 4, POP_AX, POP_AX, POP_AX, POP_AX});
            break;

        case 0x1B: // select
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
            // push word 4
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
            // push word 2
            // end:
            insts.push_back({POP_V32A, 0x66, 0x83, 0xF8, 0, JE(18), POP_AX, 0x66, 0x83, 0xF8, 2, JE(4), POP_AX, POP_AX, POP_AX, POP_AX, JMP(72), POP_AX, 0x66, 0x83, 0xF8, 2, JE(42), POP_EAX, POP_ECX, POP_DX, POP_DX, POP_DX, POP_DX, POP_DX, PUSH_V64, JMP(22), POP_EAX, POP_CX, POP_CX, POP_CX, PUSH_V32});
            break;

        // variable instructions
        case 0x20: // local.get
            // push rbp
            // mov rax, rsp
            // mov rsp, rbp
            // mov rbp, rax
            // sub rsp, [localidx * 10 + 10]
            // pop ax
            // cmp ax, 4
            // je v64
            // pop ax
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // mov rsp, rbp
            // pop rbp
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            // jmp end
            // v64:
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // shl ecx, 16
            // pop cx
            // mov rsp, rbp
            // pop rbp
            // push cx
            // shr ecx, 16
            // push cx
            // push ax
            // shr eax, 16
            // push ax
            // push word 4
            // end:
            localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);
            localidx *= 10;
            localidx += 10;
            insts.push_back({0x55, 0x48, 0x89, 0xE0, 0x48, 0x89, 0xEC, 0x48, 0x89, 0xC5, 0x48, 0x81, 0xEC, (uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24), POP_AX, 0x66, 0x83, 0xF8, 4, JE(28), POP_AX, POP_V32A, 0x48, 0x89, 0xEC, 0x5D, PUSH_V32, JMP(36), POP_EAX, POP_ECX, 0x48, 0x89, 0xEC, 0x5D, PUSH_V64});
            i += 4;
            break;

        // numeric instructions
        case 0x41: // i32.const
            // mov ax, n[15:0]
            // push ax
            // mov ax, n[31:16]
            // push ax
            // push word 2
            insts.push_back({MOV_AX, buf.at(i + 1), buf.at(i + 2), PUSH_AX, MOV_AX, buf.at(i + 3), buf.at(i + 4), PUSH_AX, V32});
            i += 4;
            break;

        case 0x42: // i64.const
            // mov ax, n[15:0]
            // push ax
            // mov ax, n[31:16]
            // push ax
            // mov ax, n[47:32]
            // push ax
            // mov ax, n[63:48]
            // push ax
            // push word 4
            insts.push_back({MOV_AX, buf.at(i + 1), buf.at(i + 2), PUSH_AX, MOV_AX, buf.at(i + 3), buf.at(i + 4), PUSH_AX, MOV_AX, buf.at(i + 5), buf.at(i + 6), PUSH_AX, MOV_AX, buf.at(i + 7), buf.at(i + 8), PUSH_AX, V64});
            i += 8;
            break;

        case 0x43: // f32.const
            // mov ax, n[15:0]
            // push ax
            // mov ax, n[31:16]
            // push ax
            // push word 2
            insts.push_back({MOV_AX, buf.at(i + 1), buf.at(i + 2), PUSH_AX, MOV_AX, buf.at(i + 3), buf.at(i + 4), PUSH_AX, V32});
            i += 4;
            break;

        case 0x44: // f64.const
            // mov ax, n[15:0]
            // push ax
            // mov ax, n[31:16]
            // push ax
            // mov ax, n[47:32]
            // push ax
            // mov ax, n[63:48]
            // push ax
            // push word 4
            insts.push_back({MOV_AX, buf.at(i + 1), buf.at(i + 2), PUSH_AX, MOV_AX, buf.at(i + 3), buf.at(i + 4), PUSH_AX, MOV_AX, buf.at(i + 5), buf.at(i + 6), PUSH_AX, MOV_AX, buf.at(i + 7), buf.at(i + 8), PUSH_AX, V64});
            i += 8;
            break;

        case 0x45: // i32.eqz
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
            insts.push_back({POP_V32A, 0x83, 0xF8, 0, JE(6), PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x46: // i32.eq
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
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC8, JE(6), PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x47: // i32.ne
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
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC8, JE(6), PUSH(1), JMP(4), PUSH(0), PUSH(0), V32});
            break;

        case 0x48: // i32.lt_s
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jl true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x7C, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x49: // i32.lt_u
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jb true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x72, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4A: // i32.gt_s
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jg true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x7F, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4B: // i32.gt_u
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // ja true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x77, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4C: // i32.le_s
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jle true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x7E, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4D: // i32.le_u
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jbe true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x76, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4E: // i32.ge_s
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jge true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x7D, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x4F: // i32.ge_u
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp ecx, eax
            // jae true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC1, 0x73, 6, PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x50: // i64.eqz
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
            insts.push_back({POP_V64A, 0x39, 0xC8, JNE(11), 0x83, 0xF8, 0, JNE(6), PUSH(1), JMP(4), PUSH(0), PUSH(0), V32});
            break;

        case 0x51: // i64.eq
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
            insts.push_back({POP_V64A, POP_V64B, 0x39, 0xD0, JNE(10), 0x39, 0xD9, JNE(6), PUSH(1), JMP(4), PUSH(0), PUSH(0), V32});
            break;

        case 0x52: // i64.ne
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
            // jne true
            // cmp ecx, ebx
            // jne true
            // push word 0
            // jmp end
            // true:
            // push word 1
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V64A, POP_V64B, 0x39, 0xD0, JNE(10), 0x39, 0xD9, JNE(6), PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x53: // i64.lt_s
            break;

        case 0x54: // i64.lt_u
            break;

        case 0x55: // i64.gt_s
            break;

        case 0x56: // i64.gt_u
            break;

        case 0x57: // i64.le_s
            break;

        case 0x58: // i64.le_u
            break;

        case 0x59: // i64.ge_s
            break;

        case 0x5A: // i64.ge_u
            break;

        case 0x5B: // f32.eq
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
            insts.push_back({POP_V32A, POP_V32B, 0x39, 0xC8, JE(6), PUSH(0), JMP(4), PUSH(1), PUSH(0), V32});
            break;

        case 0x5C: // f32.ne
            break;

        case 0x5D: // f32.lt
            break;

        case 0x5E: // f32.gt
            break;

        case 0x5F: // f32.le
            break;

        case 0x60: // f32.ge
            break;

        case 0x61: // f64.eq
            break;

        case 0x62: // f64.ne
            break;

        case 0x63: // f64.lt
            break;

        case 0x64: // f64.gt
            break;

        case 0x65: // f64.le
            break;

        case 0x66: // f64.ge
            break;

        case 0x67: // i32.clz
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // cmp eax, 0
            // je true
            // bsr eax, eax
            // mov cx, 31
            // sub cx, ax
            // push cx
            // jmp end
            // true:
            // push word 32
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, 0x83, 0xF8, 0, JE(14), 0x0F, 0xBD, 0xC0, 0x66, 0xB9, 31, 0, 0x66, 0x29, 0xC1, PUSH_CX, JMP(4), PUSH(32), PUSH(0), V32});
            break;

        case 0x68: // i32.ctz
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // cmp eax, 0
            // je true
            // bsf eax, eax
            // push ax
            // jmp end
            // true:
            // push word 32
            // end:
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, 0x83, 0xF8, 0, JE(7), 0x0F, 0xBC, 0xC0, PUSH_AX, JMP(4), PUSH(32), PUSH(0), V32});
            break;

        case 0x69: // i32.popcnt
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // popcnt eax, eax
            // push ax
            // push word 0
            // push word 2
            insts.push_back({POP_V32A, 0xF3, 0x0F, 0xB8, 0xC0, PUSH_AX, PUSH(0), V32});
            break;

        case 0x6A: // i32.add
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
            insts.push_back({POP_V32A, POP_V32B, 0x01, 0xC8, PUSH_V32});
            break;

        case 0x6B: // i32.sub
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
            insts.push_back({POP_V32B, POP_V32A, 0x29, 0xC8, PUSH_V32});
            break;

        case 0x6C: // i32.mul
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // pop cx
            // shl ecx, 16
            // pop cx
            // imul eax, ecx
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            insts.push_back({POP_V32A, POP_V32B, 0x0F, 0xAF, 0xC1, PUSH_V32});
            break;

        case 0x6D: // i32.div_s
            break;

        case 0x6E: // i32.div_u
            break;

        case 0x6F: // i32.rem_s
            break;

        case 0x70: // i32.rem_u
            break;

        case 0x71: // i32.and
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
            insts.push_back({POP_V32A, POP_V32B, 0x21, 0xC8, PUSH_V32});
            break;

        case 0x72: // i32.or
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
            insts.push_back({POP_V32A, POP_V32B, 0x09, 0xC8, PUSH_V32});
            break;

        case 0x73: // i32.xor
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
            insts.push_back({POP_V32A, POP_V32B, 0x31, 0xC8, PUSH_V32});
            break;

        case 0x74: // i32.shl
            // mov cx, 32
            // pop dx
            // pop dx
            // pop ax
            // idiv cx
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // mov cl, dl
            // shl eax, cl
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            insts.push_back({0x66, 0xB9, 32, 0, POP_DX, POP_DX, POP_AX, 0x66, 0xF7, 0xF9, POP_V32A, 0x88, 0xD1, 0xD3, 0xE0, PUSH_V32});
            break;

        case 0x75: // i32.shr_s
            // mov cx, 32
            // pop dx
            // pop dx
            // pop ax
            // idiv cx
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // mov cl, dl
            // sar eax, cl
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            insts.push_back({0x66, 0xB9, 32, 0, POP_DX, POP_DX, POP_AX, 0x66, 0xF7, 0xF9, POP_V32A, 0x88, 0xD1, 0xD3, 0xF8, PUSH_V32});
            break;

        case 0x76: // i32.shr_u
            // mov cx, 32
            // pop dx
            // pop dx
            // pop ax
            // idiv cx
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // mov cl, dl
            // shr eax, cl
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            insts.push_back({0x66, 0xB9, 32, 0, POP_DX, POP_DX, POP_AX, 0x66, 0xF7, 0xF9, POP_V32A, 0x88, 0xD1, 0xD3, 0xE8, PUSH_V32});
            break;

        case 0x77: // i32.rotl
            break;

        case 0x78: // i32.rotr
            break;

        case 0x79: // i64.clz
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp eax, 0
            // je highzero
            // bsr eax, eax
            // mov cx, 31
            // sub cx, ax
            // push cx
            // jmp end
            // highzero:
            // cmp ecx, 0
            // je lowzero
            // bsr ecx, ecx
            // mov ax, 63
            // sub ax, cx
            // push ax
            // jmp end
            // lowzero:
            // push word 64
            // end:
            // push word 0
            // push word 0
            // push word 0
            // push word 4
            insts.push_back({POP_V64A, 0x83, 0xF8, 0, JE(14), 0x0F, 0xBD, 0xC0, 0x66, 0xB9, 31, 0, 0x66, 0x29, 0xC1, PUSH_CX, JMP(23), 0x83, 0xF9, 0, JE(14), 0x0F, 0xBD, 0xC9, 0x66, 0xB8, 63, 0, 0x66, 0x29, 0xC8, PUSH_AX, JMP(4), PUSH(64), PUSH(0), PUSH(0), PUSH(0), V64});
            break;

        case 0x7A: // i64.ctz
            // pop ax
            // pop ax
            // shl eax, 16
            // pop ax
            // pop cx
            // shl ecx, 16
            // pop cx
            // cmp eax, 0
            // je highzero
            // bsf eax, eax
            // add ax, 32
            // push ax
            // jmp end
            // highzero:
            // cmp ecx, 0
            // je lowzero
            // bsf ecx, ecx
            // push cx
            // jmp end
            // lowzero:
            // push word 64
            // end:
            // push word 0
            // push word 0
            // push word 0
            // push word 4
            insts.push_back({POP_V64A, 0x83, 0xF8, 0, JE(11), 0x0F, 0xBC, 0xC0, 0x66, 0x83, 0xC0, 32, PUSH_AX, JMP(16), 0x83, 0xF9, 0, JE(7), 0x0F, 0xBC, 0xC9, PUSH_CX, JMP(4), PUSH(64), PUSH(0), PUSH(0), PUSH(0), V64});
            break;

        case 0x7B: // i64.popcnt
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
            insts.push_back({POP_V64A, 0xF3, 0x0F, 0xB8, 0xC0, 0xF3, 0x0F, 0xB8, 0xC9, 0x01, 0xC8, PUSH_AX, PUSH(0), PUSH(0), PUSH(0), V64});
            break;

        case 0x7C: // i64.add
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
            insts.push_back({POP_V64A, POP_V64B, 0x01, 0xD9, 0x11, 0xD0, PUSH_V64});
            break;

        case 0x7D: // i64.sub
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
            insts.push_back({POP_V64A, POP_V64B, 0x29, 0xD9, 0x19, 0xD0, PUSH_V64});
            break;

        case 0x7E: // i64.mul
            break;

        case 0x7F: // i64.div_s
            break;

        case 0x80: // i64.div_u
            break;

        case 0x81: // i64.rem_s
            break;

        case 0x82: // i64.rem_u
            break;

        case 0x83: // i64.and
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
            insts.push_back({POP_V64A, POP_V64B, 0x21, 0xD0, 0x21, 0xD9, PUSH_V64});
            break;

        case 0x84: // i64.or
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
            insts.push_back({POP_V64A, POP_V64B, 0x09, 0xD0, 0x09, 0xD9, PUSH_V64});
            break;

        case 0x85: // i64.xor
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
            insts.push_back({POP_V64A, POP_V64B, 0x31, 0xD0, 0x31, 0xD9, PUSH_V64});
            break;

        case 0x86: // i64.shl
            break;

        case 0x87: // i64.shr_s
            break;

        case 0x88: // i64.shr_u
            break;

        case 0x89: // i64.rotl
            break;

        case 0x8A: // i64.rotr
            break;

        case 0x8B: // f32.abs
            break;

        case 0x8C: // f32.neg
            break;

        case 0x8D: // f32.ceil
            break;

        case 0x8E: // f32.floor
            break;

        case 0x8F: // f32.trunc
            break;

        case 0x90: // f32.nearest
            break;

        case 0x91: // f32.sqrt
            break;

        case 0x92: // f32.add
            break;

        case 0x93: // f32.sub
            break;

        case 0x94: // f32.mul
            break;

        case 0x95: // f32.div
            break;

        case 0x96: // f32.min
            break;

        case 0x97: // f32.max
            break;

        case 0x98: // f32.copysign
            break;

        case 0x99: // f64.abs
            break;

        case 0x9A: // f64.neg
            break;

        case 0x9B: // f64.ceil
            break;

        case 0x9C: // f64.floor
            break;

        case 0x9D: // f64.trunc
            break;

        case 0x9E: // f64.nearest
            break;

        case 0x9F: // f64.sqrt
            break;

        case 0xA0: // f64.add
            break;

        case 0xA1: // f64.sub
            break;

        case 0xA2: // f64.mul
            break;

        case 0xA3: // f64.div
            break;

        case 0xA4: // f64.min
            break;

        case 0xA5: // f64.max
            break;

        case 0xA6: // f64.copysign
            break;

        case 0xA7: // i32.wrap_i64
            // pop ax
            // pop ax
            // pop ax
            // push word 2
            insts.push_back({POP_AX, POP_AX, POP_AX, V32});
            break;

        case 0xA8: // i32.trunc_f32_s
            break;

        case 0xA9: // i32.trunc_f32_u
            break;

        case 0xAA: // i32.trunc_f64_s
            break;

        case 0xAB: // i32.trunc_f64_u
            break;

        case 0xAC: // i64.extend_i32_s
            // pop ax
            // pop ax
            // push ax
            // and ax, 0x8000
            // cmp ax, 0
            // je zero
            // push word 0xffff
            // push word 0xffff
            // jmp end
            // zero:
            // push word 0
            // push word 0
            // end:
            // push word 4
            insts.push_back({POP_AX, POP_AX, PUSH_AX, 0x66, 0x25, 0, 128, 0x66, 0x83, 0xF8, 0, JE(8), 0x66, 0x6A, 0xFF, 0x66, 0x6A, 0xFF, JMP(8), PUSH(0), PUSH(0), V64});
            break;

        case 0xAD: // i64.extend_i32_u
            // pop ax
            // push word 0
            // push word 0
            // push word 4
            insts.push_back({POP_AX, PUSH(0), PUSH(0), V64});
            break;

        case 0xAE: // i64.trunc_f32_s
            break;

        case 0xAF: // i64.trunc_f32_u
            break;

        case 0xB0: // i64.trunc_f64_s
            break;

        case 0xB1: // i64.trunc_f64_u
            break;

        case 0xB2: // f32.convert_i32_s
            break;

        case 0xB3: // f32.convert_i32_u
            break;

        case 0xB4: // f32.convert_i64_s
            break;

        case 0xB5: // f32.convert_i64_u
            break;

        case 0xB6: // f32.demote_f64
            break;

        case 0xB7: // f64.convert_i32_s
            break;

        case 0xB8: // f64.convert_i32_u
            break;

        case 0xB9: // f64.convert_i64_s
            break;

        case 0xBA: // f64.convert_i64_u
            break;

        case 0xBB: // f64.promote_f32
            break;

        case 0xBC: // i32.reinterpret_f32
            break;

        case 0xBD: // i64.reinterpret_f64
            break;

        case 0xBE: // f32.reinterpret_i32
            break;

        case 0xBF: // f64.reinterpret_i64
            break;

        default:
            break;
        }
    }

    return concat({}, insts);
}
