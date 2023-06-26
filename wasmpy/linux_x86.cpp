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

        loadLocals = {};

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
