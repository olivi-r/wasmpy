#include "common.h"

static PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "snapshot_preview1",
    NULL,
    -1,
    methods};

PyMODINIT_FUNC PyInit_snapshot_preview1(void)
{
    return PyModule_Create(&module);
}

const wasi_rights_t wasi_rights_sock_accept = 1 << 29;
const wasi_whence_t wasi_whence_set = 0;
const wasi_whence_t wasi_whence_cur = 1;
const wasi_whence_t wasi_whence_end = 2;

API wasi_errno_t wasi_sock_accept(wasi_fd_t fd, wasi_fdflags_t flags, wasi_fd_t *opened_sock)
{
    return wasi_errno_nosys;
}
