import ctypes
import tempfile

import wasmpy
import wasmpy.nativelib
import wasmpy.util


_memory_files = []


class ResultVoid(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte)]
    _pack_ = 1


class ResultI32(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_int32)]
    _pack_ = 1


class ResultF32(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_float)]
    _pack_ = 1


class ResultI64(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_int64)]
    _pack_ = 1


class ResultF64(ctypes.Structure):
    _fields_ = [("errno", ctypes.c_byte), ("value", ctypes.c_double)]
    _pack_ = 1


def create_memory(min_pages, max_pages):
    file = tempfile.TemporaryFile()
    file.write(b"\0" * 65536 * min_pages)  # populate to initial size
    return (
        wasmpy.nativelib.create_memory(file.fileno(), min_pages, max_pages),
        file,  # store the file with module so it isn't garbage collected
    )


def create_global(mut, globaltype, expr):
    assert len(expr), "Missing initializer"
    ret, address = create_function(globaltype, bytes(expr), standalone=True)
    value = ctypes.CFUNCTYPE(ret)(address)()

    if isinstance(value.contents, ResultF32):
        value = ctypes.cast(value, ctypes.POINTER(ResultI32))

    elif isinstance(value.contents, ResultF64):
        value = ctypes.cast(value, ctypes.POINTER(ResultI64))

    return wasmpy.nativelib.append_global(value.contents.value, mut, globaltype)


def get_global_object(offset, globaltype):
    if globaltype == 0x7F:
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_int32))

    if globaltype == 0x7E:
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_int64))

    if globaltype == 0x7D:
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_float))

    if globaltype == 0x7C:
        return ctypes.cast(offset, ctypes.POINTER(ctypes.c_double))


def create_function(ret, code, arg=b"", local=b"", standalone=False):
    address = wasmpy.nativelib.create_function(
        ret,
        code,
        arg,
        local,
        wasmpy.util.consumes,
        wasmpy.util.signatures,
        standalone,
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

    if standalone:
        return ret, address

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
            params.append(ctypes.c_float)

        if a == 0x7C:
            params.append(ctypes.c_double)

    return params, param_clear


def wrap_function(func, param_clear, raw=False):
    if raw:
        wrapper = eval(
            f"lambda {', '.join(param_clear)}: func({', '.join(param_clear)})",
            {"func": func},
        )

    else:
        # ensure result structs
        wrapper = eval(
            f"lambda {', '.join(param_clear)}: ensure(func({', '.join(param_clear)}))",
            {"ensure": ensure, "func": func},
        )

    wrapper.addr = ctypes.cast(func, ctypes.c_void_p).value
    return wrapper


def ensure(result):
    if result.contents.errno == 0:
        if not isinstance(result.contents, ResultVoid):
            return result.contents.value

    elif result.contents.errno == 1:
        raise wasmpy.Trap("unreachable")

    elif result.contents.errno == 2:
        raise wasmpy.Trap("integer divide by zero")

    elif result.contents.errno == 3:
        raise wasmpy.Trap("integer overflow")

    elif result.contents.errno == 4:
        raise wasmpy.Trap("integer modulo by zero")

    elif result.contents.errno == 5:
        raise wasmpy.Trap("unrepresentable truncation result")

    elif result.contents.errno == 6:
        raise wasmpy.Trap("unimplemented instruction")
