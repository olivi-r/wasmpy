#ifndef MEMORIES_H
#define MEMORIES_H

#include "nativelib.hpp"

struct memory_t;

extern std::vector<memory_t *> memories;

void unmapMemory(memory_t *mem);
int32_t growMemory(memory_t *mem, uint32_t n);
void closeMemory(memory_t *mem);

PyObject *createMemory(PyObject *self, PyObject *args);
PyObject *loadMemoryPage(PyObject *self, PyObject *args);

#endif
