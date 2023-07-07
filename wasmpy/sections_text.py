from sexpdata import Symbol

valtypes = {
    Symbol("i32"): 0x7F,
    Symbol("i64"): 0x7E,
    Symbol("f32"): 0x7D,
    Symbol("f64"): 0x7C,
}


def flatten(expr: list) -> list:
    if isinstance(expr, (list, tuple)):
        expr = [flatten(sub_expr) for sub_expr in expr]

    return (
        expr.value()
        if hasattr(expr, "value") and not expr.value().startswith("$")
        else expr
    )


def unfold(expr: list) -> list:
    if isinstance(expr, list):
        if len(expr) > 1:
            new_expr = [unfold(i) for i in expr]
            offset = None
            for idx, i in enumerate(new_expr):
                if isinstance(i, list):
                    offset = idx
                    break

            if offset is not None:
                new_expr = [
                    term for sub_expr in new_expr[offset:] for term in sub_expr
                ] + new_expr[:offset]

            return new_expr

        else:
            return unfold(expr[0])

    else:
        return expr


def read_type(expr: list) -> tuple:
    typeidx = None
    if len(expr) == 3:
        typeidx = expr[1]

    assert expr[-1][0].value() == "func", f"Malformed function type '{expr}'"
    t1 = []
    t2 = []
    for sub_expr in expr[-1][1:]:
        if sub_expr[0].value() == "param":
            t1 += flatten(sub_expr[1:])

        if sub_expr[0].value() == "result":
            t2 += flatten(sub_expr[1:])

    for ts in (t1, t2):
        for i, t in enumerate(ts):
            t_sym = Symbol(t)
            if t_sym in valtypes:
                ts[i] = valtypes[t_sym]

    return typeidx, (t1, t2)


def read_func(expr: list) -> tuple:
    funcidx = None
    export = None
    typeidx = None
    off = 1
    if isinstance(expr[off], Symbol):
        funcidx = expr[off]
        assert funcidx.value().startswith("$")
        off += 1

    if (
        isinstance(expr[off], list)
        and isinstance(expr[off][0], Symbol)
        and expr[off][0].value() == "export"
    ):
        export = expr[off][1]
        off += 1

    if (
        isinstance(expr[off], list)
        and isinstance(expr[off][0], Symbol)
        and expr[off][0].value() == "type"
    ):
        typeidx = expr[off][1]
        off += 1

    params = []
    param_ids = []
    for sub_expr in expr[off:]:
        if (
            isinstance(sub_expr, list)
            and isinstance(sub_expr[0], Symbol)
            and sub_expr[0].value() == "param"
        ):
            if sub_expr[1].value().startswith("$"):
                param_ids.append(sub_expr[1])
                assert sub_expr[2] in valtypes
                params.append(valtypes[sub_expr[2]])

            else:
                for valtype in sub_expr[1:]:
                    assert valtype in valtypes
                    params.append(valtypes[valtype])
                    param_ids.append(None)

            off += 1

    result = None
    if (
        isinstance(expr[off], list)
        and isinstance(expr[off][0], Symbol)
        and expr[off][0].value() == "result"
    ):
        result = valtypes[expr[off][1]]
        off += 1

    if result is not None:
        typeuse = (params, [result])

    elif params:
        typeuse = (params, [])

    else:
        typeuse = None

    body = []
    for sub_expr in expr[off:]:
        body.append(flatten(sub_expr))

    body = unfold(body)

    for i, term in enumerate(body):
        if term in param_ids:
            body[i] = param_ids.index(term)

    return (
        funcidx,
        export,
        {
            "typeidx": typeidx,
            "typeuse": typeuse,
            "locals": (),
            "body": body,
        },
    )


def read_global(expr: list) -> tuple:
    globalidx = None
    export = None
    globaltype = None
    mutable = "const"
    off = 1
    if isinstance(expr[off], Symbol) and expr[off].value().startswith("$"):
        globalidx = expr[off]
        off += 1

    if isinstance(expr[off], list) and expr[off][0].value() == "export":
        export = expr[off][1]
        off += 1

    if isinstance(expr[off], Symbol):
        globaltype = valtypes[expr[off]]
        off += 1

    elif isinstance(expr[off], list) and expr[off][0].value() == "mut":
        globaltype = valtypes[expr[off][1]]
        mutable = "mut"
        off += 1

    init = expr[off:]
    return (
        globalidx,
        export,
        {"type": globaltype, "mutable": mutable, "init": flatten(init)},
    )


def read_export(expr: list) -> list:
    name = expr[1]
    type = expr[2][0].value()
    idx = expr[2][1]
    return {"name": name, "type": type, "idx": idx}
