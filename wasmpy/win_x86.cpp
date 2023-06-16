#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <Windows.h>
#include "x86/opcodes.h"

std::vector<void (*)()> registeredFuncs = {};

void freeFuncs()
{
    for (int i = 0; i < registeredFuncs.size(); i++)
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

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    char plat, ret;
    const char *codebuf;
    Py_ssize_t codelen;
    if (!PyArg_ParseTuple(args, "bby#", &plat, &ret, &codebuf, &codelen))
        return NULL;

    bytes ret_v64;
    if (plat == 4)
        ret_v64 = {POP_AX, POP_EDX, POP_EAX, 0xC3};

    else if (plat == 8)
        ret_v64 = {POP_V32A, 0x48, 0xC1, 0xE0, 16, POP_AX, 0x48, 0xC1, 0xE0, 16, POP_AX, 0xC3};

    else
        return NULL;

    bytes code(codebuf, codelen + codebuf);

    bytes cleanupCode = {};
    char *returnType = NULL;

    switch (ret)
    {
    case 0x7F:
        cleanupCode = concat(cleanupCode, {{POP_V32A, 0xC3}});
        returnType = "i32";
        break;

    case 0x7E:
        cleanupCode = concat(cleanupCode, {ret_v64});
        returnType = "i64";
        break;

    case 0x7D:
        cleanupCode = concat(cleanupCode, {{POP_V32A, 0xC3}});
        returnType = "f32";
        break;

    case 0x7C:
        cleanupCode = concat(cleanupCode, {ret_v64});
        returnType = "f64";
        break;

    default:
        break;
    }

    auto func = writeFunction(concat(decodeFunc(code), {cleanupCode}));
    registeredFuncs.push_back(func);

    if (returnType == NULL)
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
