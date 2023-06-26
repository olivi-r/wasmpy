from .native import create_function
from .values import read_uint
from .sections import *
import importlib

# extend to add future binary formats
preamble = [b"\0asm\x01\0\0\0"]

sects = [i for j in range(1, 12) for i in (0, j)] + [0]


def read_module(buffer):
    assert preamble[0] == buffer.read(8), "Invalid magic number or version."

    module = {
        "custom": (),
        "types": (),
        "imports": (),
        "funcs": (),
        "tables": (),
        "mems": (),
        "globals": (),
        "exports": (),
        "start": None,
        "elem": (),
        "data": (),
    }

    typeidx = None
    code = ()

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
                module["custom"] += (read_customsec(buffer, length),)

            if id == 1:
                module["types"] = read_typesec(buffer)

            if id == 2:
                module["imports"] = read_importsec(buffer)

            if id == 3:
                typeidx = read_funcsec(buffer)

            if id == 4:
                module["tables"] = read_tablesec(buffer)

            if id == 5:
                module["mems"] = read_memsec(buffer)

            if id == 6:
                module["globals"] = read_globalsec(buffer)

            if id == 7:
                module["exports"] = read_exportsec(buffer)

            if id == 8:
                module["start"] = read_startsec(buffer)

            if id == 9:
                module["elem"] = read_elemsec(buffer)

            if id == 10:
                code = read_codesec(buffer)

            if id == 11:
                module["data"] = read_datasec(buffer)

    except IndexError as i:
        pass

    for i in module["imports"]:
        mod = importlib.import_module(i["module"])
        if i["desc"][0] == "func":
            module["funcs"] += (
                {
                    "type": module["types"][i["desc"][1]],
                    "obj": mod.call(i["name"]),
                },
            )

    module["funcs"] += tuple(
        {
            "type": module["types"][typeidx[i]],
            "locals": t,
            "body": e,
            "obj": create_function(
                module["types"][typeidx[i]][1][0],
                bytes(k for j in e for k in j[1:]),
                bytes(module["types"][typeidx[i]][0]),
                bytes(t),
            ),
        }
        for i, (t, e) in enumerate(code)
    )

    for e in module["exports"]:
        if e["desc"][0] == "func":
            e["obj"] = module["funcs"][e["desc"][1]]["obj"]

    if module["start"] is not None:
        module["start"] = module["funcs"][module["start"]]["obj"]

    return module
