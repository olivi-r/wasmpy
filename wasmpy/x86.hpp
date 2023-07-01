#ifndef X86_H
#define X86_H

#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <vector>
#include <cstdint>
#include <cstddef>

#ifdef linux
#include <sys/mman.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

typedef std::vector<uint8_t> bytes;

extern std::vector<void (*)()> registeredFuncs;
extern bytes globalTable;

bytes concat(bytes v0, std::vector<bytes> vn);
void freeFuncs();
bytes regParam32(const char *argbuf, Py_ssize_t arglen);
PyObject *createFunction(PyObject *self, PyObject *args);
PyObject *appendGlobal(PyObject *self, PyObject *args);

#endif
