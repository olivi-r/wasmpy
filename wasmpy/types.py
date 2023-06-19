from .values import get_vec_len, read_uint

type_map = {b"\x7f": "i32", b"\x7e": "i64", b"\x7d": "f32", b"\x7c": "f64"}


def read_valtype(buffer: object) -> str:
    """Read a value type from bufer."""
    # https://www.w3.org/TR/wasm-core-1/#value-types%E2%91%A2
    try:
        return type_map[buffer.read(1)]

    except KeyError:
        raise TypeError("Invalid value type.")


def read_resulttype(buffer: object) -> str:
    """Read a result type from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#result-types%E2%91%A2
    if buffer.read(1) == b"\x40":
        # no result type
        return None

    buffer.seek(-1, 1)
    return read_valtype(buffer)


def read_functype(buffer: object) -> tuple:
    """Read a function type from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#function-types%E2%91%A4
    assert (
        buffer.read(1) == b"\x60"
    ), f"Malformed function type at {buffer.tell() - 1}"
    t1 = tuple([buffer.read(1)[0] for _ in range(get_vec_len(buffer))])
    t2 = tuple([buffer.read(1)[0] for _ in range(get_vec_len(buffer))])
    return t1, t2


def read_limits(buffer: object) -> tuple:
    """Read limits from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#limits%E2%91%A6
    try:
        flag = buffer.read(1)[0]
        assert flag in range(2)
        if flag:
            return tuple(read_uint(buffer, 32) for _ in range(2))

        else:
            return (read_uint(buffer, 32),)

    except (IndexError, AssertionError):
        raise TypeError("Invalid limits.")


def read_memtype(buffer: object) -> tuple:
    """Attempt to read a memory type from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#memory-types%E2%91%A4
    return read_limits(buffer)


def read_tabletype(buffer: object) -> tuple:
    """Attempt to read a table type from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#table-types%E2%91%A4
    if buffer.read(1) != b"\x70":
        raise TypeError("Invalid element type.")

    return read_limits(buffer)


def read_globaltype(buffer: object) -> tuple:
    """Attempt to read a global type from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#global-types%E2%91%A4
    t = read_valtype(buffer)
    try:
        mut = buffer.read(1)[0]
        assert mut in range(2)
        if mut:
            return "var", t

        return "const", t

    except (IndexError, AssertionError):
        raise TypeError("Invalid global type.")
