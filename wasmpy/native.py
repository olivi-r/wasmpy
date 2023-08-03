from . import nativelib, util
import ctypes


class ResultVoid(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte)]
    _pack_ = 1


class ResultI32(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_uint32)]
    _pack_ = 1


class ResultF32(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_float)]
    _pack_ = 1


class ResultI64(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_uint64)]
    _pack_ = 1


class ResultF64(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_double)]
    _pack_ = 1


def create_global(mut, globaltype, expr):
    assert len(expr), "Missing initializer"
    value = create_function(globaltype, bytes(expr))()
    return nativelib.append_global(value, mut, globaltype)


def get_global_object(offset, globaltype):
    if globaltype in (0x7F, 0x7D):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_int32))

    if globaltype in (0x7E, 0x7C):
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_int64))


def create_function(ret, code, arg=b"", local=b""):
    address = nativelib.create_function(
        ret,
        code,
        arg,
        local,
        util.consumes,
        util.signatures,
    )

    params, param_clear = gen_params(arg)

    if ret == 0x7F:
        ret = ctypes.POINTER(ResultI32)

    elif ret == 0x7E:
        ret = ctypes.POINTER(ResultI64)

    elif ret == 0x7D:
        ret = ctypes.POINTER(ResultF32)

    elif ret == 0x7C:
        ret = ctypes.POINTER(ResultF64)

    else:
        ret = ctypes.POINTER(ResultVoid)

    return {
        "ret": ret,
        "params": params,
        "address": address,
        "param_clear": param_clear,
    }


def gen_params(arg):
    params = []
    param_clear = []
    for i, a in enumerate(arg):
        param_clear.append(f"p{i}")
        if a == 0x7F:
            params.append(ctypes.c_uint32)

        if a == 0x7E:
            params.append(ctypes.c_uint64)

        if a == 0x7D:
            params.append(ctypes.c_uint32)

        if a == 0x7C:
            params.append(ctypes.c_uint64)

    return params, param_clear


def wrap_function(func, param_clear, raw=False):
    if raw:
        wrapper = eval(
            f"lambda {', '.join(['_'] + param_clear)}: func({', '.join(param_clear)})",
            {"func": func},
        )

    else:
        # ensure result structs
        wrapper = eval(
            f"lambda {', '.join(['_'] + param_clear)}: ensure(func({', '.join(param_clear)}))",
            {"ensure": ensure, "func": func},
        )

    wrapper.func = func
    return wrapper


def ensure(result):
    if result.contents.errno == 0:
        if not isinstance(result.contents, ResultVoid):
            return result.contents.value

    elif result.contents.errno == 1:
        raise RuntimeError("unreachable")

    elif result.contents.errno == 2:
        raise ZeroDivisionError("division by zero")

    elif result.contents.errno == 3:
        raise RuntimeError("division overflow")

    elif result.contents.errno == 4:
        raise ZeroDivisionError("integer modulo by zero")

    elif result.contents.errno == 5:
        raise FloatingPointError("unrepresentable truncation result")
