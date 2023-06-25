import platform
import struct
import ctypes


if platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64"):
    if platform.system() == "Windows":
        from . import win_x86 as nativelib

    elif platform.system() == "Linux":
        from . import linux_x86 as nativelib


def create_function(ret, code, arg=b"", local=b""):
    func, ret = nativelib.create_function(
        struct.calcsize("P"), ret, code, arg, local
    )

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
