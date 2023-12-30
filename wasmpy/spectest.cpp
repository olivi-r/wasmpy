#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 3
#include <Python.h>
#include <iostream>
#include <cstdint>
#include <type_traits>

const int32_t global_i32 = 666;
const int64_t global_i64 = 666;
const float global_f32 = 666;
const double global_f64 = 666;

void print0() {}

template <typename T>
void print1(T val)
{
    std::cout << val;
    if (std::is_same<T, int32_t>::value)
        std::cout << ": i32";
    else if (std::is_same<T, int64_t>::value)
        std::cout << ": i64";
    else if (std::is_same<T, float>::value)
        std::cout << ": f32";
    else if (std::is_same<T, double>::value)
        std::cout << ": f64";
    std::cout << std::endl;
}

template <typename T0, typename T1>
void print2(T0 val0, T1 val1)
{
    print1<T0>(val0);
    print1<T1>(val1);
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
    PyObject_SetAttrString(m, "global_i32", PyLong_FromVoidPtr((void *)&global_i32));
    PyObject_SetAttrString(m, "global_i64", PyLong_FromVoidPtr((void *)&global_i64));
    PyObject_SetAttrString(m, "global_f32", PyLong_FromVoidPtr((void *)&global_f32));
    PyObject_SetAttrString(m, "global_f64", PyLong_FromVoidPtr((void *)&global_f64));
    PyObject_SetAttrString(m, "print", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print0)));
    PyObject_SetAttrString(m, "print_i32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print1<int32_t>)));
    PyObject_SetAttrString(m, "print_i64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print1<int64_t>)));
    PyObject_SetAttrString(m, "print_f32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print1<float>)));
    PyObject_SetAttrString(m, "print_f64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print1<double>)));
    PyObject_SetAttrString(m, "print_i32_f32", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print2<int32_t, float>)));
    PyObject_SetAttrString(m, "print_f64_f64", PyLong_FromVoidPtr(reinterpret_cast<void *>(&print2<double, double>)));
    return m;
}
