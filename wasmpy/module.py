import io, os, struct, sys
from .values import read_uint
from .sections import *

# extend to add future binary formats
preamble = [b"\0asm\x01\0\0\0"]

sects = [i for j in range(1, 12) for i in (0, j)] + [0]


def read_module(buffer):
    assert preamble[0] == buffer.read(8), "Invalid magic number or version."

    module = {}

    # attempt to read sections
    upto = 0
    try:
        while upto < 23:
            id = buffer.read(1)[0]
            assert id < 12
            length = read_uint(buffer, 32)

            while sects[upto] != id:
                upto += 1
                if upto == 22:
                    break

            if sects[upto] != id:
                break

            if not id:
                CustomSection.read(buffer, length)

            if id == 1:
                module["types"] = TypeSection.read(buffer)

            if id == 2:
                module["imports"] = ImportSection.read(buffer)

            if id == 3:
                FunctionSection.read(buffer)

            if id == 4:
                module["tables"] = TableSection.read(buffer)

            if id == 5:
                module["mems"] = MemorySection.read(buffer)

            if id == 6:
                module["globals"] = GlobalSection.read(buffer)

            if id == 7:
                module["exports"] = ExportSection.read(buffer)

            if id == 8:
                module["start"] = StartSection.read(buffer)

            if id == 9:
                module["elem"] = ElementSection.read(buffer)

            if id == 10:
                CodeSection.read(buffer)

            if id == 11:
                DataSection.read(buffer)

    except IndexError:
        # eof
        pass

    return module
