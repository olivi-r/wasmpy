from . import win_x86
import struct
import ctypes

def create_function(ret, code):
    func, ret = win_x86.create_function(struct.calcsize("P"), ret, code)
    if ret == "i32":
        ret = ctypes.c_uint32
    
    elif ret == "i64":
        ret = ctypes.c_uint64

    elif ret == "f32":
        ret = ctypes.c_float

    elif ret == "f64":
        ret = ctypes.c_double
    
    else:
        ret = ctypes.c_void_p

    return ctypes.CFUNCTYPE(ret)(func)
