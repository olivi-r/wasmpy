from . import opcodes
import ctypes, platform, struct


class Result32(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_uint32)]
    _pack_ = 1


class Result64(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_uint64)]
    _pack_ = 1


if platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64"):
    from . import x86 as nativelib
    from .x86 import flush_globals, write_globals


def create_global(mut, globaltype, expr):
    assert len(expr), "Missing initializer"
    value = create_function(globaltype, bytes(expr))()
    return nativelib.append_global(value, mut, globaltype)


def get_global_object(offset, globaltype):
    if globaltype in (0x7F, 0x7D):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_uint32))

    if globaltype in (0x7E, 0x7C):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_uint64))


def create_function(ret, code, arg=b"", local=b""):
    func = nativelib.create_function(
        struct.calcsize("P"),
        ret,
        code,
        arg,
        local,
        opcodes.consumes,
        opcodes.signatures,
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
        ret = ctypes.c_int32

    elif ret == 0x7E:
        ret = ctypes.c_int64

    elif ret == 0x7D:
        ret = ctypes.c_float

    elif ret == 0x7C:
        ret = ctypes.c_double

    else:
        ret = None

    func = ctypes.CFUNCTYPE(ret, *params)(func)
    func.__doc__ = doc
    return func
