#define WASI_UNSTABLE
#include "common.h"

static PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "unstable",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_unstable(void)
{
    return PyModule_Create(&module);
}

const wasi_whence_t wasi_whence_cur = 0;
const wasi_whence_t wasi_whence_end = 1;
const wasi_whence_t wasi_whence_set = 2;
