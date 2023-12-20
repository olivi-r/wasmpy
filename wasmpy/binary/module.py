import ctypes
import importlib
import os

import wasmpy.binary.sections
import wasmpy.binary.values
import wasmpy.native
import wasmpy.nativelib
import wasmpy.util


# extend to add future binary formats
preamble = [b"\0asm\x01\0\0\0"]

sects = [i for j in range(1, 12) for i in (0, j)] + [0]


def read_module(buffer: object) -> dict:
    assert preamble[0] == buffer.read(8), "Invalid magic number or version."

    mod_dict = {
        "custom": (),
        "types": (),
        "imports": (),
        "funcs": [],
        "tables": (),
        "mem": (),
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
            length = wasmpy.binary.values.read_uint(buffer, 32)

            while sects[upto] != id:
                upto += 1
                if upto == 22:
                    break

            if sects[upto] != id:
                break

            if not id:
                mod_dict["custom"] += (
                    wasmpy.binary.sections.read_customsec(buffer, length),
                )

            if id == 1:
                mod_dict["types"] = wasmpy.binary.sections.read_typesec(buffer)

            if id == 2:
                mod_dict["imports"] = wasmpy.binary.sections.read_importsec(
                    buffer
                )

            if id == 3:
                typeidx = wasmpy.binary.sections.read_funcsec(buffer)

            if id == 4:
                mod_dict["tables"] = wasmpy.binary.sections.read_tablesec(
                    buffer
                )

            if id == 5:
                mod_dict["mem"] = wasmpy.binary.sections.read_memsec(buffer)

            if id == 6:
                mod_dict["globals"] = wasmpy.binary.sections.read_globalsec(
                    buffer
                )

            if id == 7:
                mod_dict["exports"] = wasmpy.binary.sections.read_exportsec(
                    buffer
                )

            if id == 8:
                mod_dict["start"] = wasmpy.binary.sections.read_startsec(buffer)

            if id == 9:
                mod_dict["elem"] = wasmpy.binary.sections.read_elemsec(buffer)

            if id == 10:
                code = wasmpy.binary.sections.read_codesec(buffer)

            if id == 11:
                mod_dict["data"] = wasmpy.binary.sections.read_datasec(buffer)

    except IndexError as i:
        pass

    for i in mod_dict["imports"]:
        if i["module"] == "capi":
            mod_dict["funcs"] += (getattr(ctypes.pythonapi, i["name"]),)

        else:
            mod = importlib.import_module(i["module"])
            if i["desc"][0] == "func":
                mod_dict["funcs"] += (
                    {
                        "type": mod_dict["types"][i["desc"][1]],
                        "obj": getattr(mod, i["name"]),
                    },
                )

    for g in mod_dict["globals"]:
        g["offset"] = wasmpy.native.create_global(
            g["globaltype"][0] == "mut", g["globaltype"][1], g["expr"]
        )

    global_offset = wasmpy.nativelib.write_globals()
    for g in mod_dict["globals"]:
        g["offset"] += global_offset
        g["obj"] = [
            g["globaltype"][0],
            wasmpy.native.get_global_object(g["offset"], g["globaltype"][1]),
        ]

    if mod_dict["mem"]:
        min_pages = mod_dict["mem"][0]
        max_pages = 65536
        if len(mod_dict["mem"]) == 2:
            max_pages = mod_dict["mem"][1]

        mod_dict["mem"] = wasmpy.native.create_memory(min_pages, max_pages)

    mod_dict["funcs"] += [
        wasmpy.native.create_function(
            mod_dict["types"][typeidx[i]][1][0],
            bytes(body),
            bytes(mod_dict["types"][typeidx[i]][0]),
            bytes(locals),
        )
        for i, (locals, body) in enumerate(code)
    ]

    function_base_addr = wasmpy.nativelib.write_function_page()
    for funcidx, func in enumerate(mod_dict["funcs"]):
        if isinstance(func, dict):
            obj = ctypes.CFUNCTYPE(func["ret"], *func["params"])(
                function_base_addr + func["address"]
            )
            mod_dict["funcs"][funcidx] = wasmpy.native.wrap_function(
                obj, func["param_clear"]
            )

    wasmpy.nativelib.flush_globals()
    wasmpy.nativelib.destruct_standalone()

    for e in mod_dict["exports"]:
        if e["desc"][0] == "func":
            e["obj"] = mod_dict["funcs"][e["desc"][1]]

        elif e["desc"][0] == "global":
            e["obj"] = mod_dict["globals"][e["desc"][1]]["obj"]

        elif e["desc"][0] == "mem":
            e["obj"] = mod_dict["mem"][1].raw
            e["obj"].seek(0)

    if mod_dict["start"] is not None:
        mod_dict["start"] = mod_dict["funcs"][mod_dict["start"]]

    return wasmpy.util.create_module(mod_dict)
