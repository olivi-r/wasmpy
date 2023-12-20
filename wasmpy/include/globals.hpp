#ifndef GLOBALS_H
#define GLOBALS_H

#include "nativelib.hpp"

extern bytes globalTypes;
extern uint64_t globalTableAddr;
extern std::vector<int> globalMut;

PyObject *appendGlobal(PyObject *self, PyObject *args);
PyObject *writeGlobals(PyObject *self, PyObject *args);
PyObject *flushGlobals(PyObject *self, PyObject *args);

#endif
