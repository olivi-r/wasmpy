from .instructions import read_expr
from .utils import get_vec_len, read_name, read_uint
from .types import GlobalType, MemoryType, TableType, Type, FunctionType


class CustomSection(Type):
    @classmethod
    def read(cls, buffer: object, length: int) -> None:
        """Read a custom section from buffer."""
        """Does not return anything as custom sections are dropped."""
        buffer.read(length)


class TypeSection(Type):
    def __init__(self, ft: list):
        self.data = ft

    @classmethod
    def read(cls, buffer: object) -> object:
        ft = []
        for _ in range(get_vec_len(buffer)):
            ft.append(FunctionType.read(buffer))

        return cls(ft)


class ImportSection(Type):
    def __init__(self, im: list):
        self.data = im

    @classmethod
    def read(cls, buffer: object) -> object:
        im = []
        try:
            for _ in range(get_vec_len(buffer)):
                import_ = {"mod": read_name(buffer), "nm": read_name(buffer)}
                flag = buffer.read(1)[0]
                assert flag in range(4)
                if not flag:
                    import_["d"] = ["func", read_uint(buffer, 32)]

                elif flag == 1:
                    import_["d"] = ["table", TableType.read(buffer)]

                elif flag == 2:
                    import_["d"] = ["mem", MemoryType.read(buffer)]

                elif flag == 3:
                    import_["d"] = ["global", GlobalType.read(buffer)]

                im.append(import_)

        except (IndexError, AssertionError):
            raise TypeError("Invalid import section.")

        return cls(im)


class FunctionSection(Type):
    def __init__(self, x: list):
        self.data = x

    @classmethod
    def read(cls, buffer: object) -> object:
        x = []
        for _ in range(get_vec_len(buffer)):
            x.append(read_uint(buffer, 32))

        return cls(x)


class TableSection(Type):
    def __init__(self, tab: list):
        self.data = tab

    @classmethod
    def read(cls, buffer: object) -> object:
        tab = []
        for _ in range(get_vec_len(buffer)):
            tab.append({"type": TableType.read(buffer)})

        return cls(tab)


class MemorySection(Type):
    def __init__(self, mem: list):
        self.data = mem

    @classmethod
    def read(cls, buffer: object) -> object:
        mem = []
        for _ in range(get_vec_len(buffer)):
            mem.append({"type": MemoryType.read(buffer)})

        return cls(mem)


class GlobalSection(Type):
    def __init__(self, glob: list):
        self.data = glob

    @classmethod
    def read(cls, buffer: object) -> object:
        glob = []
        for _ in range(get_vec_len(buffer)):
            glob.append({"gt": GlobalType.read(buffer), "e": read_expr(buffer)})

        return cls(glob)


class ExportSection(Type):
    def __init__(self, ex: list):
        self.data = ex

    @classmethod
    def read(cls, buffer: object) -> object:
        ex = []
        for _ in range(get_vec_len(buffer)):
            export = {"nm": read_name(buffer)}
            desc = buffer.read(1)[0]
            assert desc in range(4)
            if not desc:
                export["d"] = ["func", read_uint(buffer, 32)]

            if desc == 1:
                export["d"] = ["table", read_uint(buffer, 32)]

            if desc == 2:
                export["d"] = ["mem", read_uint(buffer, 32)]

            if desc == 3:
                export["d"] = ["global", read_uint(buffer, 32)]

            ex.append(export)

        return cls(ex)


class StartSection(Type):
    def __init__(self, x: int):
        self.data = x

    @classmethod
    def read(cls, buffer: object) -> object:
        return cls(read_uint(buffer, 32))


class ElementSection(Type):
    def __init__(self, seg: list):
        self.data = seg

    @classmethod
    def read(cls, buffer: object) -> object:
        seg = []
        for _ in range(get_vec_len(buffer)):
            elem = {"table": read_uint(buffer, 32), "e": read_expr(buffer), "y": []}
            for _ in range(get_vec_len(buffer)):
                elem["y"].append(read_uint(buffer, 32))

            seg.append(elem)

        return cls(seg)


class CodeSection(Type):
    def __init__(self):
        self.data

    @classmethod
    def read(cls, buffer: object) -> object:
        pass


class DataSection(Type):
    def __init__(self):
        self.data

    @classmethod
    def read(cls, buffer: object) -> object:
        pass
