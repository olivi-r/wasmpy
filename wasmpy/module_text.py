from .instructions import read_expr_text
from .sections_text import *
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
        "exports": (),
        "start": None,
        "elem": (),
        "data": (),
    }

    type_ids = []
    global_ids = []

    for expr in data:
        if isinstance(expr, list):
            if expr[0].value() == "type":
                typeidx, type = read_type(expr)
                module["types"].append(type)
                type_ids.append(typeidx)

            elif expr[0].value() == "func":
                module["funcs"].append(read_func(expr))

            elif expr[0].value() == "global":
                globalidx, global_ = read_global(expr)
                module["globals"].append(global_)
                global_ids.append(globalidx)

    for func in module["funcs"]:
        if func["typeidx"] is not None:
            if func["typeuse"] is not None:
                assert (
                    module["types"][type_ids.index(func["typeidx"])]
                    == func["typeuse"]
                ), "type mismatch"

            func["type"] = module["types"][type_ids.index(func["typeidx"])]

        else:
            assert func["typeuse"] is not None, "missing type"
            func["type"] = func["typeuse"]

        for i, term in enumerate(func["body"]):
            if term in global_ids:
                func["body"][i] = global_ids.index(term)

        func["body"] = read_expr_text(func["body"])

    return module
