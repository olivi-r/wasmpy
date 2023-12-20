#ifndef LIB_H
#define LIB_H
#include "nativelib.hpp"

#ifdef ARCH_X86
int64_t func_div64_s(int64_t j1, int64_t j2);

uint64_t func_div64_u(uint64_t i1, uint64_t i2);

int64_t func_rem64_s(int64_t j1, int64_t j2);

uint64_t func_rem64_u(uint64_t i1, uint64_t i2);
#endif

bytes regParam(const char *argbuf, Py_ssize_t arglen);

#endif
