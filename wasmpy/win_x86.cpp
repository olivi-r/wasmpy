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
    const char *codebuf, *retbuf;
    Py_ssize_t codelen, retlen;
    if (!PyArg_ParseTuple(args, "y#y#", &retbuf, &retlen, &codebuf, &codelen))
        return NULL;

    bytes code(codebuf, codelen + codebuf);

    bytes cleanupCode = {};
    char *returnType = NULL;

    if (retlen == 1)
    {
        switch (retbuf[0])
        {
        case 0x7F:
            cleanupCode = concat(cleanupCode, {{POP_V32A, 0xC3}});
            returnType = "i32";
            break;

        case 0x7E:
            cleanupCode = concat(cleanupCode, {{POP_V64A, 0xC3}});
            returnType = "i64";
            break;

        case 0x7D:
            cleanupCode = concat(cleanupCode, {{POP_V32A, 0xC3}});
            returnType = "f32";
            break;

        case 0x7C:
            cleanupCode = concat(cleanupCode, {{POP_V64A, 0xC3}});
            returnType = "f64";
            break;

        default:
            break;
        }
    }

    if (retlen > 1 || returnType == NULL)
        return NULL;

    auto func = writeFunction(concat(decodeFunc(code), {cleanupCode}));
    registeredFuncs.push_back(func);

    if (retlen == 1)
        return Py_BuildValue("OU#", PyLong_FromSize_t((size_t)func), returnType, 3);

    return Py_BuildValue("OO", PyLong_FromSize_t((size_t)func), Py_None);
}

static PyMethodDef methods[] = {
    {"create_function", createFunction, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "x86_win",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_win_x86()
{
    Py_AtExit(&freeFuncs);
    return PyModule_Create(&module);
}
