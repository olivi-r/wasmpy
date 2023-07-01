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

bytes concat(bytes v0, std::vector<bytes> vn);

#endif
