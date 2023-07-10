from . import instructions, module, native, sections_text
import io, shutil
import sexpdata


def read_module(buffer: object) -> dict:
    try:
        data = sexpdata.load(buffer)

    except AssertionError:
        # Handle case where (module ...) omitted
        buffer.seek(0)
        new_buffer = io.StringIO()
        new_buffer.write("(module\n")
        shutil.copyfileobj(buffer, new_buffer)
        new_buffer.write("\n)")
        new_buffer.seek(0)
        data = sexpdata.load(new_buffer)
        new_buffer.close()

    mod_dict = {
        "custom": (),
        "types": [],
        "imports": (),
        "funcs": [],
        "tables": (),
        "mems": (),
        "globals": [],
        "exports": [],
        "start": None,
        "elem": (),
        "data": (),
    }

    type_ids = []
    func_ids = []
    global_ids = []

    for expr in data:
        if isinstance(expr, list):
            if expr[0].value() == "type":
                typeidx, type = sections_text.read_type(expr)
                mod_dict["types"].append(type)
                type_ids.append(typeidx)

            elif expr[0].value() == "func":
                funcidx, export, func = sections_text.read_func(expr)
                mod_dict["funcs"].append(func)
                func_ids.append(funcidx)
                if export is not None:
                    mod_dict["exports"].append(
                        {
                            "name": export,
                            "type": "func",
                            "idx": len(mod_dict["funcs"]) - 1,
                        }
                    )

            elif expr[0].value() == "global":
                globalidx, export, global_ = sections_text.read_global(expr)
                mod_dict["globals"].append(global_)
                global_ids.append(globalidx)
                if export is not None:
                    mod_dict["exports"].append(
                        {
                            "name": export,
                            "type": "global",
                            "idx": len(mod_dict["globals"]) - 1,
                        }
                    )

            elif expr[0].value() == "export":
                mod_dict["exports"].append(sections_text.read_export(expr))

    for g in mod_dict["globals"]:
        for i, term in enumerate(g["init"]):
            if term in global_ids:
                g["init"][i] = global_ids.index(term)

        g["init"] = instructions.read_expr_text(g["init"])
        g["offset"] = native.create_global(
            g["mutable"] == "mut", g["type"], g["init"]
        )

    global_offset = native.write_globals()
    for globalidx, g in enumerate(mod_dict["globals"]):
        g["offset"] += global_offset
        g["obj"] = [
            g["mutable"],
            native.get_global_object(g["offset"], g["type"]),
        ]

        mod_dict["globals"][globalidx] = g["obj"]

    for funcidx, func in enumerate(mod_dict["funcs"]):
        if func["typeidx"] is not None:
            if func["typeidx"] in type_ids:
                func["typeidx"] = type_ids.index(func["typeidx"])

            if func["typeuse"] is not None:
                assert (
                    mod_dict["types"][func["typeidx"]] == func["typeuse"]
                ), "type mismatch"

            func["type"] = mod_dict["types"][func["typeidx"]]

        else:
            assert func["typeuse"] is not None, "missing type"
            func["type"] = func["typeuse"]

        for i, term in enumerate(func["body"]):
            if term in global_ids:
                func["body"][i] = global_ids.index(term)

        func["body"] = instructions.read_expr_text(func["body"])
        if not func["type"][1]:
            func["type"][1].append(0x40)

        func["obj"] = native.create_function(
            func["type"][1][0], bytes(func["body"]), bytes(func["type"][0])
        )

        mod_dict["funcs"][funcidx] = func["obj"]

    native.native.flush_globals()

    for e in mod_dict["exports"]:
        if isinstance(e["idx"], sexpdata.Symbol):
            if e["type"] == "func":
                e["idx"] = func_ids.index[e["idx"]]

            elif e["type"] == "global":
                e["idx"] = global_ids.index(e["idx"])

        if e["type"] == "func":
            e["obj"] = mod_dict["funcs"][e["idx"]]

        if e["type"] == "global":
            e["obj"] = mod_dict["globals"][e["idx"]]

    return module.create_module(mod_dict)
