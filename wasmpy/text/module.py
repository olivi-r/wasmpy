import ctypes

import sexpdata

import wasmpy.native
import wasmpy.nativelib
import wasmpy.text.instructions
import wasmpy.text.sections
import wasmpy.util


def read_module(buffer: object) -> dict:
    original = buffer.read()
    buffer.close()
    comment_depth = 0
    stripped = ""
    skip = False
    for i, value in enumerate(original):
        if skip:
            skip = False
            continue

        if i != len(original) - 1:
            if value == "(" and original[i + 1] == ";":
                comment_depth += 1
                skip = True

            elif value == ";" and original[i + 1] == ")":
                comment_depth -= 1
                skip = True

            elif comment_depth == 0:
                stripped += value

        elif comment_depth == 0:
            stripped += value

    try:
        data = sexpdata.loads(stripped)

    except AssertionError:
        # Handle case where (module ...) omitted
        stripped = "(module\n" + stripped + "\n)"
        data = sexpdata.loads(stripped)

    mod_dict = {
        "custom": (),
        "types": [],
        "imports": (),
        "funcs": [],
        "tables": (),
        "mem": (),
        "globals": [],
        "exports": [],
        "start": None,
        "elem": (),
        "data": (),
    }

    type_ids = []
    global_ids = []

    import_funcs = {"funcs": [], "ids": []}
    funcs = {"funcs": [], "ids": []}

    for expr in data:
        if isinstance(expr, list):
            if expr[0].value() == "type":
                typeidx, type = wasmpy.text.sections.read_type(expr)
                mod_dict["types"].append(type)
                type_ids.append(typeidx)

            elif expr[0].value() == "import":
                (
                    type,
                    mod,
                    name,
                    importidx,
                    desc,
                ) = wasmpy.text.sections.read_import(expr)
                if type == "func":
                    if mod == "capi":
                        import_funcs["ids"].append(importidx)
                        desc["obj"] = getattr(ctypes.pythonapi, name)
                        import_funcs["funcs"].append(desc)

                    # TODO: .wasm imports

            elif expr[0].value() == "func":
                funcidx, func = wasmpy.text.sections.read_func(expr)
                funcs["funcs"].append(func)
                funcs["ids"].append(funcidx)

            elif expr[0].value() == "memory":
                if mod_dict["mem"]:
                    raise ValueError("Multiple memories defined")

                _, min_pages, max_pages = wasmpy.text.sections.read_memory(expr)
                memory = wasmpy.native.create_memory(min_pages, max_pages)
                mod_dict["mem"] = memory

            elif expr[0].value() == "global":
                globalidx, export, global_ = wasmpy.text.sections.read_global(expr)
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
                mod_dict["exports"].append(wasmpy.text.sections.read_export(expr))

            elif expr[0].value() == "start":
                if mod_dict["start"] is not None:
                    raise ValueError("Duplicate start symbol")

                try:
                    mod_dict["start"] = expr[1]

                except IndexError:
                    raise ValueError("Invalid start symbol")

    func_ids = import_funcs["ids"] + funcs["ids"]
    mod_dict["funcs"] = import_funcs["funcs"] + funcs["funcs"]

    for g in mod_dict["globals"]:
        for i, term in enumerate(g["init"]):
            if term in global_ids:
                g["init"][i] = global_ids.index(term)

        g["init"] = wasmpy.text.instructions.read_expr_text(g["init"])
        g["offset"] = wasmpy.native.create_global(
            g["mutable"] == "mut", g["type"], g["init"]
        )

    global_offset = wasmpy.nativelib.write_globals()
    for globalidx, g in enumerate(mod_dict["globals"]):
        g["offset"] += global_offset
        g["obj"] = [
            g["mutable"],
            wasmpy.native.get_global_object(g["offset"], g["type"]),
        ]

        mod_dict["globals"][globalidx] = g["obj"]

    for funcidx, func in enumerate(mod_dict["funcs"]):
        if "export" in func and func["export"] is not None:
            mod_dict["exports"].append(
                {
                    "name": func["export"],
                    "type": "func",
                    "idx": funcidx,
                }
            )

        if func["typeidx"] is not None:
            if func["typeidx"] in type_ids:
                func["typeidx"] = type_ids.index(func["typeidx"])

            if func["typeuse"] is not None:
                assert (
                    mod_dict["types"][func["typeidx"]] == func["typeuse"]
                ), "type mismatch"

            func["type"] = mod_dict["types"][func["typeidx"]]

        else:
            if func["typeuse"] is None:
                func["typeuse"] = ([], [])

            func["type"] = func["typeuse"]

        if "obj" not in func:
            for i, term in enumerate(func["body"]):
                if term in global_ids:
                    func["body"][i] = global_ids.index(term)

            func["body"] = wasmpy.text.instructions.read_expr_text(func["body"])
            if not func["type"][1]:
                func["type"][1].append(0x40)

            func["_obj"] = wasmpy.native.create_function(
                func["type"][1][0],
                bytes(func["body"]),
                bytes(func["type"][0]),
                bytes(func["locals"]),
            )

        else:
            if not hasattr(func["obj"], "func"):
                params, param_clear = wasmpy.native.gen_params(func["type"][0])
                func["obj"].argtypes = params
                if not func["type"][1]:
                    func["obj"].restype = None

                elif func["type"][1][0] == 0x7F:
                    func["obj"].restype = ctypes.c_uint32

                elif func["type"][1][0] == 0x7E:
                    func["obj"].restype = ctypes.c_uint64

                elif func["type"][1][0] == 0x7D:
                    func["obj"].restype = ctypes.c_float

                elif func["type"][1][0] == 0x7C:
                    func["obj"].restype = ctypes.c_double

                func["obj"] = wasmpy.native.wrap_function(
                    func["obj"], param_clear, True
                )

    function_base_addr = wasmpy.nativelib.write_function_page()
    for funcidx, func in enumerate(mod_dict["funcs"]):
        if "_obj" in func:
            obj = ctypes.CFUNCTYPE(func["_obj"]["ret"], *func["_obj"]["params"])(
                function_base_addr + func["_obj"]["address"]
            )
            func["obj"] = wasmpy.native.wrap_function(obj, func["_obj"]["param_clear"])

        mod_dict["funcs"][funcidx] = func["obj"]

    if mod_dict["start"] is not None:
        try:
            if mod_dict["start"] in type_ids:
                mod_dict["start"] = mod_dict["funcs"][type_ids.index(mod_dict["start"])]

            else:
                mod_dict["start"] = mod_dict["funcs"][mod_dict["start"]]

        except (IndexError, TypeError):
            raise ValueError("Invalid start symbol")

    wasmpy.nativelib.flush_globals()
    wasmpy.nativelib.destruct_standalone()

    for e in mod_dict["exports"]:
        if isinstance(e["idx"], sexpdata.Symbol):
            if e["type"] == "func":
                e["idx"] = func_ids.index(e["idx"])

            elif e["type"] == "global":
                e["idx"] = global_ids.index(e["idx"])

        if e["type"] == "func":
            e["obj"] = mod_dict["funcs"][e["idx"]]

        if e["type"] == "global":
            e["obj"] = mod_dict["globals"][e["idx"]]

    return wasmpy.util.create_module(mod_dict)
