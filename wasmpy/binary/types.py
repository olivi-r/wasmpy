import wasmpy.binary.values


type_map = {b"\x7f": "i32", b"\x7e": "i64", b"\x7d": "f32", b"\x7c": "f64"}


def read_valtype(buffer: object) -> str:
    """Read a value type from bufer."""
    try:
        return type_map[buffer.read(1)]

    except KeyError:
        raise TypeError("Invalid value type.")


def read_resulttype(buffer: object) -> str:
    """Read a result type from buffer."""
    if buffer.read(1) == b"\x40":
        # no result type
        return None

    buffer.seek(-1, 1)
    return read_valtype(buffer)


def read_functype(buffer: object) -> tuple:
    """Read a function type from buffer."""
    assert (
        buffer.read(1) == b"\x60"
    ), f"Malformed function type at {buffer.tell() - 1}"
    t1 = [
        buffer.read(1)[0]
        for _ in range(wasmpy.binary.values.get_vec_len(buffer))
    ]
    t2 = [
        buffer.read(1)[0]
        for _ in range(wasmpy.binary.values.get_vec_len(buffer))
    ]
    if len(t2) == 0:
        t2.append(0x40)

    return t1, t2


def read_limits(buffer: object) -> tuple:
    """Read limits from buffer."""
    try:
        flag = buffer.read(1)[0]
        assert flag in range(2)
        if flag:
            lims = tuple(
                wasmpy.binary.values.read_uint(buffer, 32) for _ in range(2)
            )
            assert lims[0] <= lims[1], "Invalid limits"
            return lims

        else:
            return (wasmpy.binary.values.read_uint(buffer, 32),)

    except (IndexError, AssertionError):
        raise TypeError("Invalid limits.")


def read_memtype(buffer: object) -> tuple:
    """Attempt to read a memory type from buffer."""
    return read_limits(buffer)


def read_tabletype(buffer: object) -> tuple:
    """Attempt to read a table type from buffer."""
    if buffer.read(1) != b"\x70":
        raise TypeError("Invalid element type.")

    return read_limits(buffer)


def read_globaltype(buffer: object) -> tuple:
    """Attempt to read a global type from buffer."""
    t = buffer.read(1)[0]
    try:
        mut = buffer.read(1)[0]
        assert mut in range(2)
        if mut:
            return "mut", t

        return "const", t

    except (IndexError, AssertionError):
        raise TypeError("Invalid global type.")
