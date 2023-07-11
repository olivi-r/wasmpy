from . import module, native, sections_binary, values
import importlib


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
            length = values.read_uint(buffer, 32)

            while sects[upto] != id:
                upto += 1
                if upto == 22:
                    break

            if sects[upto] != id:
                break

            if not id:
                mod_dict["custom"] += (
                    sections_binary.read_customsec(buffer, length),
                )

            if id == 1:
                mod_dict["types"] = sections_binary.read_typesec(buffer)

            if id == 2:
                mod_dict["imports"] = sections_binary.read_importsec(buffer)

            if id == 3:
                typeidx = sections_binary.read_funcsec(buffer)

            if id == 4:
                mod_dict["tables"] = sections_binary.read_tablesec(buffer)

            if id == 5:
                mod_dict["mems"] = sections_binary.read_memsec(buffer)

            if id == 6:
                mod_dict["globals"] = sections_binary.read_globalsec(buffer)

            if id == 7:
                mod_dict["exports"] = sections_binary.read_exportsec(buffer)

            if id == 8:
                mod_dict["start"] = sections_binary.read_startsec(buffer)

            if id == 9:
                mod_dict["elem"] = sections_binary.read_elemsec(buffer)

            if id == 10:
                code = sections_binary.read_codesec(buffer)

            if id == 11:
                mod_dict["data"] = sections_binary.read_datasec(buffer)

    except IndexError as i:
        pass

    for i in mod_dict["imports"]:
        mod = importlib.import_module(i["module"])
        if i["desc"][0] == "func":
            mod_dict["funcs"] += (
                {
                    "type": mod_dict["types"][i["desc"][1]],
                    "obj": mod.call(i["name"]),
                },
            )

    for g in mod_dict["globals"]:
        g["offset"] = native.create_global(
            g["globaltype"][0] == "mut", g["globaltype"][1], g["expr"]
        )

    global_offset = native.nativelib.write_globals()
    for g in mod_dict["globals"]:
        g["offset"] += global_offset
        g["obj"] = [
            g["globaltype"][0],
            native.get_global_object(g["offset"], g["globaltype"][1]),
        ]

    mod_dict["funcs"] += [
        native.create_function(
            mod_dict["types"][typeidx[i]][1][0],
            bytes(body),
            bytes(mod_dict["types"][typeidx[i]][0]),
            bytes(locals),
        )
        for i, (locals, body) in enumerate(code)
    ]

    native.nativelib.flush_globals()

    for e in mod_dict["exports"]:
        if e["desc"][0] == "func":
            e["obj"] = mod_dict["funcs"][e["desc"][1]]

        elif e["desc"][0] == "global":
            e["obj"] = mod_dict["globals"][e["desc"][1]]["obj"]

    if mod_dict["start"] is not None:
        mod_dict["start"] = mod_dict["funcs"][mod_dict["start"]]

    return module.create_module(mod_dict)
