#define PY_SSIZE_T_CLEAN
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

bytes regParam64(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    for (int i = 0; i < (arglen < 5 ? arglen : 4); i++)
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

    return code;
}

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    char plat, ret;
    const char *codebuf, *argbuf;
    Py_ssize_t codelen, arglen;
    if (!PyArg_ParseTuple(args, "bby#y#", &plat, &ret, &codebuf, &codelen, &argbuf, &arglen))
        return NULL;

    bytes ret_v64, cleanupStack;
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

        cleanupStack = {};
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

        // mov rsp, rbp
        // pop rbp
        cleanupStack = {0x48, 0x89, 0xEC, 0x5D};
    }
    else
        return NULL;

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

    // push rbp
    // mov rbp, rsp
    bytes init = {0x55, 0x48, 0x89, 0xE5};

    auto func = writeFunction(concat(init, {regParam64(argbuf, arglen), decodeFunc(code), cleanupCode}));
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
