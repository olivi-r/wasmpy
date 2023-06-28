import sexpdata


def flatten(expr: list) -> list:
    out = []
    for sub_expr in expr:
        if hasattr(sub_expr, "value"):
            out.append(sub_expr.value())

    return out


def read_typesec(expr: list) -> tuple:
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
            if t == "i32":
                ts[i] = 0x7F

            elif t == "i64":
                ts[i] == 0x7E

            elif t == "f32":
                ts[i] == 0x7D

            elif t == "f64":
                ts[i] == 0x7C

    return t1, t2
