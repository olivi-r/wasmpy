def seq_dict(*args, offset=0):
    d = {}
    for i, arg in enumerate(args):
        d[arg] = offset + i

    return d


opcodes = {
    # control instructions
    "nop": 0x01,
    # parametric instructions
    "drop": 0x1A,
    "select": 0x1B,
}

# variable instructions
opcodes.update(
    seq_dict(
        "local.get",
        "local.set",
        "local.tee",
        "global.get",
        "global.set",
        offset=0x20,
    )
)

# numeric instructions
opcodes.update(
    seq_dict(
        "i32.const",
        "i64.const",
        "f32.const",
        "f64.const",
        "i32.eqz",
        "i32.eq",
        "i32.ne",
        "i32.lt_s",
        "i32.lt_u",
        "i32.gt_s",
        "i32.gt_u",
        "i32.le_s",
        "i32.le_u",
        "i32.ge_s",
        "i32.ge_u",
        "i64.eqz",
        "i64.eq",
        "i64.ne",
        "i64.lt_s",
        "i64.lt_u",
        "i64.gt_s",
        "i64.gt_u",
        "i64.le_s",
        "i64.le_u",
        "i64.ge_s",
        "i64.ge_u",
        "f32.eq",
        "f32.ne",
        "f32.lt",
        "f32.gt",
        "f32.le",
        "f32.ge",
        "f64.eq",
        "f64.ne",
        "f64.lt",
        "f64.gt",
        "f64.le",
        "f64.ge",
        "i32.clz",
        "i32.ctz",
        "i32.popcnt",
        "i32.add",
        "i32.sub",
        "i32.mul",
        "i32.div_s",
        "i32.div_u",
        "i32.rem_s",
        "i32.rem_u",
        "i32.and",
        "i32.or",
        "i32.xor",
        "i32.shl",
        "i32.shr_s",
        "i32.shr_u",
        "i32.rotl",
        "i32.rotr",
        "i64.clz",
        "i64.ctz",
        "i64.popcnt",
        "i64.add",
        "i64.sub",
        "i64.mul",
        "i64.div_s",
        "i64.div_u",
        "i64.rem_s",
        "i64.rem_u",
        "i64.and",
        "i64.or",
        "i64.xor",
        "i64.shl",
        "i64.shr_s",
        "i64.shr_u",
        "i64.rotl",
        "i64.rotr",
        "f32.abs",
        "f32.neg",
        "f32.ceil",
        "f32.floor",
        "f32.trunc",
        "f32.nearest",
        "f32.sqrt",
        "f32.add",
        "f32.sub",
        "f32.mul",
        "f32.div",
        "f32.min",
        "f32.max",
        "f32.copysign",
        "f64.abs",
        "f64.neg",
        "f64.ceil",
        "f64.floor",
        "f64.trunc",
        "f64.nearest",
        "f64.sqrt",
        "f64.add",
        "f64.sub",
        "f64.mul",
        "f64.div",
        "f64.min",
        "f64.max",
        "f64.copysign",
        "i32.wrap_i64",
        "i32.trunc_f32_s",
        "i32.trunc_f32_u",
        "i32.trunc_f64_s",
        "i32.trunc_f64_u",
        "i64.extend_i32_s",
        "i64.extend_i32_u",
        "i64.trunc_f32_s",
        "i64.trunc_f32_u",
        "i64.trunc_f64_s",
        "i64.trunc_f64_u",
        "f32.convert_i32_s",
        "f32.convert_i32_u",
        "f32.convert_i64_s",
        "f32.convert_i64_u",
        "f32.demote_f64",
        "f64.convert_i32_s",
        "f64.convert_i32_u",
        "f64.convert_i64_s",
        "f64.convert_i64_u",
        "f64.promote_f32",
        "i32.reinterpret_f32",
        "i64.reinterpret_f64",
        "f32.reinterpret_i32",
        "f64.reinterpret_i64",
        offset=0x41,
    )
)

prefixed = {
    "local.get": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
        "localidx += 10;\n\t\t\t",
    ),
    "local.set": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "local.tee": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "global.get": (
        "ll = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "ll *= 9;\n\t\t\t",
        "ll += globalTableAddr;\n\t\t\t",
        "lh = ll + 2;\n\t\t\t",
        "hl = ll + 4;\n\t\t\t",
        "hh = ll + 6;\n\t\t\t",
        "bits = ll + 8;\n\t\t\t",
    ),
}

consumes = {
    "local.get": 4,
    "local.set": 4,
    "local.tee": 4,
    "global.get": 4,
    "global.set": 4,
    "i32.const": 4,
    "i64.const": 8,
    "f32.const": 4,
    "f64.const": 8,
}

global_get_32 = "(uint8_t){b}, (uint8_t)({b} >> 8), (uint8_t)({b} >> 16), (uint8_t)({b} >> 24)"
global_get_64 = (
    global_get_32
    + ", (uint8_t)({b} >> 32), (uint8_t)({b} >> 40), (uint8_t)({b} >> 48), (uint8_t)({b} >> 56)"
)


replacements = {
    "local.get": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "local.set": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "local.tee": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "global.get": (
        # 64 bit replacements
        ("0, 0, 0, 255, 0, 0, 0, 0", global_get_64.format(b="ll")),
        ("255, 0, 0, 255, 0, 0, 0, 0", global_get_64.format(b="lh")),
        ("0, 255, 0, 255, 0, 0, 0, 0", global_get_64.format(b="hl")),
        ("255, 255, 0, 255, 0, 0, 0, 0", global_get_64.format(b="hh")),
        ("255, 255, 255, 255, 0, 0, 0, 0", global_get_64.format(b="bits")),
        # 32 bit replacements
        ("0, 0, 0, 255", global_get_32.format(b="ll")),
        ("255, 0, 0, 255", global_get_32.format(b="lh")),
        ("0, 255, 0, 255", global_get_32.format(b="hl")),
        ("255, 255, 0, 255", global_get_32.format(b="hh")),
        ("255, 255, 255, 255", global_get_32.format(b="bits")),
    ),
    "i32.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("255, 255", "buf.at(i + 3), buf.at(i + 4)"),
    ),
    "i64.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("0, 255", "buf.at(i + 3), buf.at(i + 4)"),
        ("255, 0", "buf.at(i + 5), buf.at(i + 6)"),
        ("255, 255", "buf.at(i + 7), buf.at(i + 8)"),
    ),
    "f32.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("255, 255", "buf.at(i + 3), buf.at(i + 4)"),
    ),
    "f64.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("0, 255", "buf.at(i + 3), buf.at(i + 4)"),
        ("255, 0", "buf.at(i + 5), buf.at(i + 6)"),
        ("255, 255", "buf.at(i + 7), buf.at(i + 8)"),
    ),
}
