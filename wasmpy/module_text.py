from .instructions import read_expr_text
from .module import create_module
from .sections_text import *
from .native import (
    create_function,
    create_global,
    write_globals,
    get_global_object,
)
import sexpdata
import shutil
import io


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

    module = {
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
                typeidx, type = read_type(expr)
                module["types"].append(type)
                type_ids.append(typeidx)

            elif expr[0].value() == "func":
                funcidx, export, func = read_func(expr)
                module["funcs"].append(func)
                func_ids.append(funcidx)
                if export is not None:
                    module["exports"].append(
                        {
                            "name": export,
                            "type": "func",
                            "idx": len(module["funcs"]) - 1,
                        }
                    )

            elif expr[0].value() == "global":
                globalidx, export, global_ = read_global(expr)
                module["globals"].append(global_)
                global_ids.append(globalidx)
                if export is not None:
                    module["exports"].append(
                        {
                            "name": export,
                            "type": "global",
                            "idx": len(module["globals"]) - 1,
                        }
                    )

            elif expr[0].value() == "export":
                module["exports"].append(read_export(expr))

    for g in module["globals"]:
        for i, term in enumerate(g["init"]):
            if term in global_ids:
                g["init"][i] = global_ids.index(term)

        g["init"] = read_expr_text(g["init"])
        g["offset"] = create_global(g["type"], g["init"])

    global_offset = write_globals()
    for globalidx, g in enumerate(module["globals"]):
        g["offset"] += global_offset
        g["obj"] = [
            g["mutable"],
            get_global_object(g["offset"], g["type"]),
        ]

        module["globals"][globalidx] = g["obj"]

    for funcidx, func in enumerate(module["funcs"]):
        if func["typeidx"] is not None:
            if func["typeuse"] is not None:
                assert (
                    module["types"][type_ids.index(func["typeidx"])]
                    == func["typeuse"]
                ), "type mismatch"

            if isinstance(func["typeidx"], sexpdata.Symbol):
                func["typeidx"] = type_ids.index(func["typeidx"])

            func["type"] = module["types"][func["typeidx"]]

        else:
            assert func["typeuse"] is not None, "missing type"
            func["type"] = func["typeuse"]

        for i, term in enumerate(func["body"]):
            if term in global_ids:
                func["body"][i] = global_ids.index(term)

        func["body"] = read_expr_text(func["body"])
        func["obj"] = create_function(
            func["type"][1][0], bytes(func["body"]), bytes(func["type"][0])
        )

        module["funcs"][funcidx] = func["obj"]

    for e in module["exports"]:
        if isinstance(e["idx"], sexpdata.Symbol):
            if e["type"] == "func":
                e["idx"] = func_ids.index[e["idx"]]

            elif e["type"] == "global":
                e["idx"] = global_ids.index(e["idx"])

        if e["type"] == "func":
            e["obj"] = module["funcs"][e["idx"]]

        if e["type"] == "global":
            e["obj"] = module["globals"][e["idx"]]

    return create_module(module)
