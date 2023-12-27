#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <iostream>
#include <cstdint>

void print_i32(int32_t val)
{
    std::cout << val << ": i32" << std::endl;
}

void print_i64(int64_t val)
{
    std::cout << val << ": i64" << std::endl;
}

void print_f32(float val)
{
    std::cout << val << ": f32" << std::endl;
}

void print_f64(double val)
{
    std::cout << val << ": f64" << std::endl;
}

void print_i32_f32(int32_t val0, float val1)
{
    print_i32(val0);
    print_f32(val1);
}

void print_f64_f64(double val0, double val1)
{
    print_f64(val0);
    print_f64(val1);
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
    PyObject_SetAttrString(m, "print_i32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_i32)));
    PyObject_SetAttrString(m, "print_i64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_i64)));
    PyObject_SetAttrString(m, "print_f32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_f32)));
    PyObject_SetAttrString(m, "print_f64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_f64)));
    PyObject_SetAttrString(m, "print_i32_f32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_i32_f32)));
    PyObject_SetAttrString(m, "print_f64_f64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print_f64_f64)));
    return m;
}
