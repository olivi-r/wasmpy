#ifndef NATIVELIB_H
#define NATIVELIB_H

#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>

#include <cstddef>
#include <cstdint>
#include <vector>

#ifdef PLATFORM_LINUX
#include <sys/mman.h>
#include <fcntl.h>
#elif PLATFORM_WINDOWS
#include <Windows.h>
#else
#error Unknown system
#endif

#ifndef ARCH_X86
#ifndef ARCH_X86_64
#error Unknown architecture
#endif
#endif

typedef std::vector<uint8_t> bytes;

extern bytes localTypes;
extern std::vector<uint32_t> localOffsets;
extern uint64_t globalTableAddr;
extern uint64_t errorPageAddr;

template <typename T>
std::vector<T> concat(std::vector<T> v0, std::vector<std::vector<T>> vn);
void *writePage(bytes data);

#include "globals.hpp"
#include "memories.hpp"

#include "opcodes.hpp"
#include "lib.hpp"

#endif
