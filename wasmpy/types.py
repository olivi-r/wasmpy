from .utils import get_vec_len, read_uint


class Type(object):
    """Generic base type."""

    def __init__(self):
        self.data = ""

    def __repr__(self):
        return self.__class__.__name__ + "(" + repr(self.data) + ")"


class ValueType(Type):
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
    def __init__(self, t: list):
        self.data = t

    @classmethod
    def read(cls, buffer: object) -> object:
        """Attempt to read a result type from buffer."""
        t = []
        try:
            for _ in range(get_vec_len(buffer)):
                t.append(ValueType.read(buffer))

            return cls(t)

        except (IndexError, AssertionError):
            raise TypeError("Invalid result type.")


class FunctionType(Type):
    def __init__(self, rt1: ResultType, rt2: ResultType):
        self.data = {rt1: rt2}

    @classmethod
    def read(cls, buffer: object) -> object:
        # try:
        assert buffer.read(1) == b"\x60"
        rt1 = ResultType.read(buffer)
        rt2 = ResultType.read(buffer)
        return cls(rt1, rt2)

        # except AssertionError:
        #     raise TypeError("Invalid function type.")


class Limits(Type):
    def __init__(self, n: int, m: int = None):
        self.data = {"min": n, "max": m}

    @classmethod
    def read(cls, buffer: object) -> object:
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
    def __init__(self, lim: Limits):
        self.data = lim

    @classmethod
    def read(cls, buffer: object) -> object:
        return cls(Limits.read(buffer))


class TableType(Type):
    def __init__(self, lim: Limits):
        # only currently supported elemtype (et) if funcref
        self.data = {"lim": lim, "et": "funcref"}

    @classmethod
    def read(cls, buffer: object) -> object:
        if buffer.read(1) != b"\x70":
            raise TypeError("Invalid element type.")

        return cls(Limits.read(buffer))


class GlobalType(Type):
    def __init__(self, m: str, t: ValueType):
        self.data = {"m": m, "t": t}

    @classmethod
    def read(cls, buffer: object) -> object:
        t = ValueType.read(buffer)
        try:
            mut = buffer.read(1)[0]
            assert mut in range(2)
            if mut:
                return cls("var", t)

            return cls("const", t)

        except (IndexError, AssertionError):
            raise TypeError("Invalid global type.")
