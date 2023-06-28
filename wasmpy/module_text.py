from .sections_text import *
import sexpdata
import shutil
import io


def read_module(buffer):
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
        "funcs": (),
        "tables": (),
        "mems": (),
        "globals": (),
        "exports": (),
        "start": None,
        "elem": (),
        "data": (),
    }

    for expr in data:
        if isinstance(expr, list):
            if expr[0].value() == "type":
                module["types"].append(read_typesec(expr))

    return module
