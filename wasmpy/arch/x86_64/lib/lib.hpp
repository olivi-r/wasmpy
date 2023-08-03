#ifndef LIB_H
#define LIB_H

#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <cstddef>
#include <cstdint>
#include <vector>

#ifdef __linux__
#include <sys/mman.h>
#elif _WIN32
#include <Windows.h>
#else
#error Unknown system
#endif

typedef std::vector<uint8_t> bytes;

extern uint64_t globalTableAddr;
extern uint64_t errorPageAddr;

template <typename T>
std::vector<T> concat(std::vector<T> v0, std::vector<std::vector<T>> vn);

#include "opcodes.hpp"

#endif
