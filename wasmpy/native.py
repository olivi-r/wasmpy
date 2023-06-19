from . import win_x86
import struct
import ctypes


def create_function(ret, code, arg=b""):
    func, ret = win_x86.create_function(struct.calcsize("P"), ret, code, arg)

    params = []
    param_doc = []
    for i, a in enumerate(arg):
        if a == 0x7F:
            params.append(ctypes.c_uint32)
            param_doc.append(f"p{i}: i32")

        if a == 0x7E:
            params.append(ctypes.c_uint64)
            param_doc.append(f"p{i}: i64")

        if a == 0x7D:
            params.append(ctypes.c_uint32)
            param_doc.append(f"p{i}: f32")

        if a == 0x7C:
            params.append(ctypes.c_uint64)
            param_doc.append(f"p{i}: f64")

    doc = f"({', '.join(param_doc)}) -> {ret}"

    if ret == "i32":
        ret = ctypes.c_uint32

    elif ret == "i64":
        ret = ctypes.c_uint64

    elif ret == "f32":
        ret = ctypes.c_float

    elif ret == "f64":
        ret = ctypes.c_double

    else:
        ret = None

    func = ctypes.CFUNCTYPE(ret, *params)(func)
    func.__doc__ = doc
    return func
