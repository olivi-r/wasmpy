#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <Windows.h>
#include "x86/opcodes.h"
#include <iostream>

std::vector<void (*)()> registeredFuncs = {};

void freeFuncs()
{
    for (int i = 0; i < registeredFuncs.size(); i++)
    {
        printf("%d\n", i);
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
    Py_buffer pybuf;
    PyArg_ParseTuple(args, "y*", &pybuf);
    uint8_t *buf = (uint8_t *)malloc(pybuf.len);
    PyBuffer_ToContiguous(buf, &pybuf, pybuf.len, 'C');
    std::vector<uint8_t> vec(buf, buf + pybuf.len);
    PyBuffer_Release(&pybuf);
    auto func = writeFunction(concat(decodeFunc(vec), {{POP_V32A, 0xC3}}));
    registeredFuncs.push_back(func);
    return PyLong_FromSize_t((size_t)func);
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

PyMODINIT_FUNC PyInit_x86_win()
{
    Py_AtExit(freeFuncs);
    return PyModule_Create(&module);
}
