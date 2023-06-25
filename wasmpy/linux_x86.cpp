#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <sys/mman.h>
#include "x86/opcodes.hpp"

std::vector<void (*)()> registeredFuncs = {};

auto writeFunction(bytes code)
{
    int pageSize = sysconf(_SC_PAGE_SIZE);
    void *buf = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    memcpy(buf, code.data(), code.size());
    mprotect(buf, code.size(), PROT_READ | PROT_EXEC);
    return reinterpret_cast<void (*)()>(buf);
}

static PyObject *createFunction(PyObject *self, PyObject *args)
{
    Py_RETURN_NONE;
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
    return PyModule_Create(&module);
}
