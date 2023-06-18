from . import win_x86
import struct
import ctypes

def create_function(ret, code, arg=b""):
    func, ret = win_x86.create_function(struct.calcsize("P"), ret, code, arg)
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

    params = []
    for a in arg:
        if a in (0x7F, 0x7D):
            params.append(ctypes.c_uint32)

        if a in (0x7E, 0x7C):
            params.append(ctypes.c_uint64)

    return ctypes.CFUNCTYPE(ret, *params)(func)
