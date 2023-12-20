#include "globals.hpp"

bytes globalTable = {};
bytes globalTypes = {};
uint64_t globalTableAddr;
std::vector<int> globalMut = {};

PyObject *appendGlobal(PyObject *self, PyObject *args)
{
    // add new global to globalTable
    unsigned long long global;
    int mut;
    char type;
    if (!PyArg_ParseTuple(args, "Kpb", &global, &mut, &type))
        return NULL;

    size_t offset = globalTable.size();
    globalMut.push_back(mut);

    // i32 or f32
    if (type == 0x7F || type == 0x7D)
        globalTable = concat(globalTable, {{(uint8_t)(global >> 24), (uint8_t)(global >> 16), (uint8_t)(global >> 8), (uint8_t)global}});

    // i64 or f64
    else if (type == 0x7E || type == 0x7C)
        globalTable = concat(globalTable, {{(uint8_t)(global >> 56), (uint8_t)(global >> 48), (uint8_t)(global >> 40), (uint8_t)(global >> 32), (uint8_t)(global >> 24), (uint8_t)(global >> 16), (uint8_t)(global >> 8), (uint8_t)global}});

    else
    {
        PyErr_SetString(PyExc_TypeError, "unrecognized global type");
        return NULL;
    }

    globalTypes.push_back(type);

    return Py_BuildValue("O", PyLong_FromSize_t(offset));
}

PyObject *writeGlobals(PyObject *self, PyObject *args)
{
    // commit globalTable to memory
    void *buf = writePage(globalTable);
    globalTableAddr = (uint64_t)buf;
    return Py_BuildValue("O", PyLong_FromVoidPtr(buf));
}

PyObject *flushGlobals(PyObject *self, PyObject *args)
{
    // flush globals ready for next module
    globalTable = {};
    globalTypes = {};
    globalMut = {};
    Py_RETURN_NONE;
}
