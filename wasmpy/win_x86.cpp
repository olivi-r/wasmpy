#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <Windows.h>
#include "x86/opcodes.h"

std::vector<void (*)()> registeredFuncs = {};

void freeFuncs()
{
    for (size_t i = 0; i < registeredFuncs.size(); i++)
    {
        VirtualFree((LPVOID)registeredFuncs.at(i), 0, MEM_RELEASE);
    }
}

auto writeFunction(bytes code)
{
    SYSTEM_INFO sysinf;
    GetSystemInfo(&sysinf);
    LPVOID buf = VirtualAlloc(nullptr, sysinf.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
    memcpy(buf, code.data(), code.size());
    DWORD dummy;
    VirtualProtect(buf, code.size(), PAGE_EXECUTE_READ, &dummy);
    return reinterpret_cast<void (*)()>(buf);
}

bytes regParam32(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    int offset = 8;
    for (Py_ssize_t i = 0; i < arglen; i++)
    {
        // mov eax, [ebp + offset]
        code = concat(code, {{0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            // push word 0
            // push word 0
            code = concat(code, {{PUSH_V32, PUSH(0), PUSH(0)}});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            offset += 4;
            // mov ecx, [ebp + offset]
            code = concat(code, {{0x8B, 0x8D, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

            // push ax
            // shr eax, 16
            // push ax
            // push cx
            // shr ecx, 16
            // push cx
            // push word 4
            code = concat(code, {{PUSH_AX, SHR_EAX, PUSH_AX, PUSH_CX, SHR_ECX, PUSH_CX, V64}});
        }

        offset += 4;
    }

    return code;
}

bytes regParam64(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    for (int i = 0; i < min(arglen, 4); i++)
    {
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i / 2 == 0)
                // push [cd]x
                // shr e[cd]x, 16
                // push [cd]x
                // push word 2
                // push word 0
                // push word 0
                code = concat(code, {{0x66, (uint8_t)(0x51 + i), 0xC1, (uint8_t)(0xE9 + i), 0x10, 0x66, (uint8_t)(0x51 + i), V32, PUSH(0), PUSH(0)}});

            else
                // push r[89]w
                // shr r[89]d, 16
                // push r[89]w
                // push word 2
                // push word 0
                // push word 0
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4E + i), 0x41, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), V32, PUSH(0), PUSH(0)}});
        }
        else if (argbuf[i] == 0x7E || argbuf[i] == 0x7C)
        {
            if (i / 2 == 0)
                // push [cd]x
                // shr r[cd]x, 16
                // push [cd]x
                // shr r[cd]x, 16
                // push [cd]x
                // shr e[cd]x, 16
                // push [cd]x
                // push word 4
                code = concat(code, {{0x66, (uint8_t)(0x51 + i), 0x48, 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), 0x48, 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), 0xC1, (uint8_t)(0xE9 + i), 16, 0x66, (uint8_t)(0x51 + i), V64}});

            else
                // push r[89]w
                // shr r[89], 16
                // push r[89]w
                // shr r[89], 16
                // push r[89]w
                // shr r[89]d, 16
                // push r[89]w
                // push word 4
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4E + i), 0x49, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), 0x49, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), 0x41, 0xC1, (uint8_t)(0xE6 + i), 16, 0x66, 0x41, (uint8_t)(0x4E + i), V64}});
        }
    }

    for (Py_ssize_t i = 0; i < arglen - 4; i++)
    {
        // mov rax, [rbp + offset]
        Py_ssize_t offset = i * 8 + 48;
        code = concat(code, {{0x48, 0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i + 4] == 0x7F || argbuf[i + 4] == 0x7D)
        {
            // push ax
            // shr eax, 16
            // push ax
            // push word 2
            // push word 0
            // push word 0
            code = concat(code, {{PUSH_V32, PUSH(0), PUSH(0)}});
        }
        else if (argbuf[i + 4] == 0x7E || argbuf[i + 4] == 0x7C)
        {
            // push ax
            // shr rax, 16
            // push ax
            // shr rax, 16
            // push ax
            // shr eax, 16
            // push ax
            // push word 4
            code = concat(code, {{PUSH_AX, 0x48, 0xC1, 0xE8, 16, PUSH_AX, 0x48, 0xC1, 0xE8, 16, PUSH_AX, SHR_EAX, PUSH_AX, V64}});
        }
    }

    return code;
}

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    char plat, ret;
    const char *codebuf, *argbuf, *localbuf;
    Py_ssize_t codelen, arglen, locallen;
    if (!PyArg_ParseTuple(args, "bby#y#y#", &plat, &ret, &codebuf, &codelen, &argbuf, &arglen, &localbuf, &locallen))
        return NULL;

    bytes ret_v64, initStack, loadLocals, cleanupStack;
    if (plat == 4)
    {
        // pop ax
        // pop dx
        // shl edx, 16
        // pop dx
        // pop ax
        // shl eax, 16
        // pop ax
        ret_v64 = {POP_AX, POP_EDX, POP_EAX};

        // push ebp
        // mov ebp, esp
        initStack = {0x55, 0x89, 0xE5};

        loadLocals = regParam32(argbuf, arglen);

        // mov esp, ebp
        // pop ebp
        cleanupStack = {0x89, 0xEC, 0x5D};
    }
    else if (plat == 8)
    {
        // pop ax
        // pop ax
        // shl eax, 16
        // pop ax
        // shl rax, 16
        // pop ax
        // shl rax, 16
        // pop ax
        ret_v64 = {POP_V32A, 0x48, 0xC1, 0xE0, 16, POP_AX, 0x48, 0xC1, 0xE0, 16, POP_AX};

        // push rbp
        // mov rbp, rsp
        initStack = {0x55, 0x48, 0x89, 0xE5};

        loadLocals = regParam64(argbuf, arglen);

        // mov rsp, rbp
        // pop rbp
        cleanupStack = {0x48, 0x89, 0xEC, 0x5D};
    }
    else
        return NULL;

    for (Py_ssize_t i = 0; i < locallen; i++)
    {
        if (localbuf[i] == 0x7F || localbuf[i] == 0x7D)
            loadLocals = concat(loadLocals, {{PUSH(0), PUSH(0), PUSH(2), PUSH(0), PUSH(0)}});

        if (localbuf[i] == 0x7E || localbuf[i] == 0x7C)
            loadLocals = concat(loadLocals, {{PUSH(0), PUSH(0), PUSH(0), PUSH(0), PUSH(4)}});
    }

    bytes code(codebuf, codelen + codebuf);

    bytes cleanupCode = {};
    char *returnType = NULL;

    switch (ret)
    {
    case 0x7F:
        cleanupCode = concat(cleanupCode, {{POP_V32A}, cleanupStack, {0xC3}});
        returnType = "i32";
        break;

    case 0x7E:
        cleanupCode = concat(cleanupCode, {ret_v64, cleanupStack, {0xC3}});
        returnType = "i64";
        break;

    case 0x7D:
        cleanupCode = concat(cleanupCode, {{POP_V32A}, cleanupStack, {0xC3}});
        returnType = "f32";
        break;

    case 0x7C:
        cleanupCode = concat(cleanupCode, {ret_v64, cleanupStack, {0xC3}});
        returnType = "f64";
        break;

    case 0x40:
        cleanupCode = concat(cleanupStack, {{0xC3}});
        returnType = "void";
        break;

    default:
        break;
    }

    if (returnType == NULL)
        return NULL;

    auto func = writeFunction(concat(initStack, {loadLocals, decodeFunc(code, plat), cleanupCode}));
    registeredFuncs.push_back(func);

    if (returnType == "void")
        return Py_BuildValue("OO", PyLong_FromSize_t((size_t)func), Py_None);

    return Py_BuildValue("OU#", PyLong_FromSize_t((size_t)func), returnType, 3);
}

static PyMethodDef methods[] = {
    {"create_function", createFunction, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "win_x86",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_win_x86()
{
    Py_AtExit(&freeFuncs);
    return PyModule_Create(&module);
}
