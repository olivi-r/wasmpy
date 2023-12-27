#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <iostream>
#include <cstdint>

template <typename T>
void print(T val)
{
    std::cout << val << std::endl;
}

PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}};

PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "spectest",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_spectest()
{
    PyObject *m = PyModule_Create(&module);
    PyObject_SetAttrString(m, "print_i32", PyLong_FromVoidPtr(&print<int32_t>));
    PyObject_SetAttrString(m, "print_i64", PyLong_FromVoidPtr(&print<int64_t>));
    PyObject_SetAttrString(m, "print_f32", PyLong_FromVoidPtr(&print<float>));
    PyObject_SetAttrString(m, "print_f64", PyLong_FromVoidPtr(&print<double>));
    return m;
}