#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <sys/mman.h>
#include "x86/opcodes.hpp"

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

std::vector<void (*)()> registeredFuncs = {};

void freeFuncs()
{
    for (size_t i = 0; i < registeredFuncs.size(); i++)
    {
        munmap((void *)registeredFuncs.at(i), PAGE_SIZE);
    }
}

auto writeFunction(bytes code)
{
    void *buf = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memcpy(buf, code.data(), code.size());
    mprotect(buf, code.size(), PROT_READ | PROT_EXEC);
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
            // push strict word 2
            // push strict word 0
            // push strict word 0
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
            // push strict word 4
            code = concat(code, {{PUSH_AX, SHR_EAX, PUSH_AX, PUSH_CX, SHR_ECX, PUSH_CX, V64}});
        }

        offset += 4;
    }

    return code;
}

bytes regParam64(const char *argbuf, Py_ssize_t arglen)
{
    bytes code = {};
    for (int i = 0; i < std::min((int)arglen, 6); i++)
    {
        if (argbuf[i] == 0x7F || argbuf[i] == 0x7D)
        {
            if (i / 2 == 0)
                // push [ds]i
                // shr e[ds]i, 16
                // push [ds]i
                // push strict word 2
                // push strict word 0
                // push strict word 0
                code = concat(code, {{0x66, (uint8_t)(0x57 - i), 0xC1, (uint8_t)(0xEF - i), 16, 0x66, (uint8_t)(0x57 - i), V32, PUSH(0), PUSH(0)}});

            else if (i / 2 == 1)
                // push [dc]x
                // shr e[dc]x, 16
                // push [dc]x
                // push strict word 2
                // push strict word 0
                // push strict word 0
                code = concat(code, {{0x66, (uint8_t)(0x54 - i), 0xC1, (uint8_t)(0xEC - i), 16, 0x66, (uint8_t)(0x54 - i), V32, PUSH(0), PUSH(0)}});

            else
                // push r[89]w
                // shr r[89]d, 16
                // push r[89]w
                // push strict word 2
                // push strict word 0
                // push strict word 0
                code = concat(code, {{0x66, 0x41, (uint8_t)(0x4C + i), 0x41, 0xC1, (uint8_t)(0xE4 + i), 16, 0x66, 0x41, (uint8_t)(0x4C + i), V32, PUSH(0), PUSH(0)}});
        }
    }

    for (Py_ssize_t i = 0; i < arglen - 6; i++)
    {
        // mov rax, [rbp + offset]
        Py_ssize_t offset = i * 8 + 16;
        code = concat(code, {{0x48, 0x8B, 0x85, (uint8_t)offset, (uint8_t)(offset >> 8), (uint8_t)(offset >> 16), (uint8_t)(offset >> 24)}});

        if (argbuf[i + 4] == 0x7F || argbuf[i + 4] == 0x7D)
        {
            // push ax
            // shr eax, 16
            // push ax
            // push strict word 2
            // push strict word 0
            // push strict word 0
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
            // push strict word 4
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

    bytes cleanupCode;
    const char *returnType;

    switch (ret)
    {
    case 0x7F:
        cleanupCode = concat({POP_V32A}, {cleanupStack, {0xC3}});
        returnType = "i32";
        break;

    case 0x7E:
        cleanupCode = concat(ret_v64, {cleanupStack, {0xC3}});
        returnType = "i64";
        break;

    case 0x7D:
        cleanupCode = concat({POP_V32A}, {cleanupStack, {0xC3}});
        returnType = "f32";
        break;

    case 0x7C:
        cleanupCode = concat(ret_v64, {cleanupStack, {0xC3}});
        returnType = "f64";
        break;

    default:
        cleanupCode = concat(cleanupStack, {{0xC3}});
        returnType = "void";
        break;
    }

    auto func = writeFunction(concat(initStack, {loadLocals, decodeFunc(code, plat), cleanupCode}));
    registeredFuncs.push_back(func);

    return Py_BuildValue("OU#", PyLong_FromSize_t((size_t)func), returnType, 3);
}

static PyMethodDef methods[] = {
    {"create_function", createFunction, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "linux_x86",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_linux_x86()
{
    Py_AtExit(&freeFuncs);
    return PyModule_Create(&module);
}
