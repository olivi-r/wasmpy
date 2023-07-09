from .opcodes import consumes, signatures
import platform
import struct
import ctypes


if platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64"):
    from . import x86 as nativelib


def create_global(globaltype, expr):
    assert len(expr), "Missing initializer"
    value = create_function(globaltype, bytes(expr))()
    return nativelib.append_global(value, globaltype)


def write_globals():
    return nativelib.write_global_table()


def get_global_object(offset, globaltype):
    if globaltype in (0x7F, 0x7D):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_uint32))

    if globaltype in (0x7E, 0x7C):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_uint64))


def create_function(ret, code, arg=b"", local=b""):
    func = nativelib.create_function(
        struct.calcsize("P"), ret, code, arg, local, consumes, signatures
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

    if ret == 0x7F:
        ret = ctypes.c_uint32

    elif ret == 0x7E:
        ret = ctypes.c_uint64

    elif ret == 0x7D:
        ret = ctypes.c_float

    elif ret == 0x7C:
        ret = ctypes.c_double

    else:
        ret = None

    func = ctypes.CFUNCTYPE(ret, *params)(func)
    func.__doc__ = doc
    return func
