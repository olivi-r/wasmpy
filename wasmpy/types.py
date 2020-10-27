from .values import get_vec_len, read_uint


class Type(object):
    """Generic base type."""

    def __init__(self):
        self.data = ""

    def __repr__(self):
        return self.__class__.__name__ + "(" + repr(self.data) + ")"


class ValueType(Type):
    """Object representing WebAssembly value types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#value-types%E2%91%A2"""

    type_map = {b"\x7f": "i32", b"\x7e": "i64", b"\x7d": "f32", b"\x7c": "f64"}

    def __init__(self, type: str):
        self.data = type

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a value type from bufer."""
        try:
            return cls(cls.type_map[buffer.read(1)])

        except KeyError:
            raise TypeError("Invalid value type.")


class ResultType(Type):
    """Object representing WebAssembly result types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#result-types%E2%91%A2"""

    def __init__(self, t: ValueType):
        self.data = t

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a result type from buffer."""
        if buffer.read(1) == b"\x40":
            # no result type
            return cls(ValueType(None))

        buffer.seek(-1, 1)
        try:
            return cls(ValueType.read(buffer))

        except TypeError:
            raise TypeError("Invalid result type.")


class FunctionType(Type):
    """Object representing WebAssembly function types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#function-types%E2%91%A4"""

    def __init__(self, rt1: tuple, rt2: tuple):
        self.data = {rt1: rt2}

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a function type from buffer."""
        assert buffer.read(1) == b"\x60"
        t1 = tuple([ValueType.read(buffer) for _ in range(get_vec_len(buffer))])
        t2 = tuple([ValueType.read(buffer) for _ in range(get_vec_len(buffer))])
        return cls(t1, t2)


class Limits(Type):
    """Object representing WebAssembly limits.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#limits%E2%91%A6"""

    def __init__(self, n: int, m: int = None):
        self.data = {"min": n, "max": m}

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read limits from buffer."""
        try:
            flag = buffer.read(1)[0]
            assert flag in range(2)
            if flag:
                return cls(*[read_uint(buffer, 32) for _ in range(2)])

            else:
                return cls(read_uint(buffer, 32))

        except (IndexError, AssertionError):
            raise TypeError("Invalid limits.")


class MemoryType(Type):
    """Object representing WebAssembly memory types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#memory-types%E2%91%A4"""

    def __init__(self, lim: Limits):
        self.data = lim

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a memory type from buffer."""
        return cls(Limits.read(buffer))


class TableType(Type):
    """Object representing WebAssembly table types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#table-types%E2%91%A4"""

    def __init__(self, lim: Limits):
        # only currently supported elemtype (et) if funcref
        self.data = {"lim": lim, "et": "funcref"}

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a table type from buffer."""
        if buffer.read(1) != b"\x70":
            raise TypeError("Invalid element type.")

        return cls(Limits.read(buffer))


class GlobalType(Type):
    """Object representing WebAssembly global types.\n\n"""

    """https://www.w3.org/TR/wasm-core-1/#global-types%E2%91%A4"""

    def __init__(self, m: str, t: ValueType):
        self.data = {"m": m, "t": t}

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a global type from buffer."""
        t = ValueType.read(buffer)
        try:
            mut = buffer.read(1)[0]
            assert mut in range(2)
            if mut:
                return cls("var", t)

            return cls("const", t)

        except (IndexError, AssertionError):
            raise TypeError("Invalid global type.")
