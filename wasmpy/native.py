from . import x86_win
import ctypes

def create_function(code):
    return ctypes.CFUNCTYPE(ctypes.c_uint32)(x86_win.create_function(code))
