from .values import get_vec_len, read_sint, read_uint, read_f32, read_f64
from .types import read_valtype


def expand_bytes(n, bits=32):
    r = n & 255, (n >> 8) & 255, (n >> 16) & 255, (n >> 24) & 255
    if bits == 32:
        return r

    return r + (
        (n >> 32) & 255,
        (n >> 40) & 255,
        (n >> 48) & 255,
        (n >> 56) & 255,
    )


def read_expr(buffer: object) -> tuple:
    """Read an expression from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#expressions%E2%91%A6
    in_ = ()
    instruction = read_instruction(buffer)
    while instruction != "end":
        in_ += (instruction,)
        instruction = read_instruction(buffer)

    return in_


def read_memarg(buffer: object) -> dict:
    """Read a memory arg from buffer."""
    # https://www.w3.org/TR/wasm-core-1/#binary-memarg
    a = read_uint(buffer, 32)
    o = read_uint(buffer, 32)
    return {"align": a, "offset": o}


def read_instruction(buffer: object) -> "str or tuple":
    """Read an instruction from buffer."""
    data = buffer.read(1)
    # 0x0B (end) and 0x05 (else) explicit instructions
    if data == b"\x0b":
        return "end"

    if data == b"\x05":
        return "else"

    # Control instructions
    # https://www.w3.org/TR/wasm-core-1/#control-instructions%E2%91%A6
    if data == b"\0":
        return "unreachable"

    if data == b"\x01":
        return "nop"

    if data == b"\x02":
        try:
            rt = read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in_ = []
        instruction = read_instruction(buffer)
        while instruction != "end":
            in_.append(instruction)
            instruction = read_instruction(buffer)

        return "block", rt, tuple(in_), "end"

    if data == b"\x03":
        try:
            rt = read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in_ = []
        instruction = read_instruction(buffer)
        while instruction != "end":
            in_.append(instruction)
            instruction = read_instruction(buffer)

        return "loop", rt, tuple(in_), "end"

    if data == b"\x04":
        try:
            rt = read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in1 = []
        instruction = read_instruction(buffer)
        while instruction not in ["end", "else"]:
            in1.append(instruction)
            instruction = read_instruction(buffer)

        in1 = tuple(in1)
        if instruction == "end":
            return "if", rt, in1, "else", (), "end"

        # "else"
        in2 = []
        instruction = read_instruction(buffer)
        while instruction != "end":
            in2.append(instruction)
            instruction = read_instruction(buffer)

        return "if", rt, in1, "else", tuple(in2), "end"

    if data == b"\x0c":
        return "br", read_uint(buffer, 32)

    if data == b"\x0d":
        return "br_if", read_uint(buffer, 32)

    if data == b"\x0e":
        l = [read_uint(buffer, 32) for _ in range(get_vec_len(buffer))]
        return "br_table", l, read_uint(buffer, 32)

    if data == b"\x0f":
        return "return"

    if data == b"\x10":
        return "call", read_uint(buffer, 32)

    if data == b"\x11":
        x = read_uint(buffer, 32)
        assert buffer.read(1) == b"\0", "Invalid instruction."
        return "call_indirect", x

    # Parametric instructions
    # https://www.w3.org/TR/wasm-core-1/#parametric-instructions%E2%91%A6
    if data == b"\x1a":
        return "drop", data[0]

    if data == b"\x1b":
        return "select", data[0]

    # Variable instructions
    # https://www.w3.org/TR/wasm-core-1/#variable-instructions%E2%91%A6
    if data == b"\x20":
        return ("local.get", data[0]) + expand_bytes(read_uint(buffer, 32))

    if data == b"\x21":
        return ("local.set", data[0]) + expand_bytes(read_uint(buffer, 32))

    if data == b"\x22":
        return ("local.tee", data[0]) + expand_bytes(read_uint(buffer, 32))

    if data == b"\x23":
        return ("global.get", data[0]) + expand_bytes(read_uint(buffer, 32))

    if data == b"\x24":
        return ("global.set", data[0]) + expand_bytes(read_uint(buffer, 32))

    # Memory instructions
    # https://www.w3.org/TR/wasm-core-1/#memory-instructions%E2%91%A6
    if data == b"\x28":
        return "i32.load", data[0], read_memarg(buffer)

    if data == b"\x29":
        return "i64.load", data[0], read_memarg(buffer)

    if data == b"\x2a":
        return "f32.load", data[0], read_memarg(buffer)

    if data == b"\x2b":
        return "f64.load", data[0], read_memarg(buffer)

    if data == b"\x2c":
        return "i32.load8_s", data[0], read_memarg(buffer)

    if data == b"\x2d":
        return "i32.load8_u", data[0], read_memarg(buffer)

    if data == b"\x2e":
        return "i32.load16_s", data[0], read_memarg(buffer)

    if data == b"\x2f":
        return "i32.load16_u", data[0], read_memarg(buffer)

    if data == b"\x30":
        return "i64.load8_s", data[0], read_memarg(buffer)

    if data == b"\x31":
        return "i64.load8_u", data[0], read_memarg(buffer)

    if data == b"\x32":
        return "i64.load16_s", data[0], read_memarg(buffer)

    if data == b"\x33":
        return "i64.load16_u", data[0], read_memarg(buffer)

    if data == b"\x34":
        return "i64.load32_s", data[0], read_memarg(buffer)

    if data == b"\x35":
        return "i64.load32_u", data[0], read_memarg(buffer)

    if data == b"\x36":
        return "i32.store", data[0], read_memarg(buffer)

    if data == b"\x37":
        return "i64.store", data[0], read_memarg(buffer)

    if data == b"\x38":
        return "f32.store", data[0], read_memarg(buffer)

    if data == b"\x39":
        return "f64.store", data[0], read_memarg(buffer)

    if data == b"\x3a":
        return "i32.store8", data[0], read_memarg(buffer)

    if data == b"\x3b":
        return "i32.store16", data[0], read_memarg(buffer)

    if data == b"\x3c":
        return "i64.store8", data[0], read_memarg(buffer)

    if data == b"\x3d":
        return "i64.store16", data[0], read_memarg(buffer)

    if data == b"\x3e":
        return "i64.store32", data[0], read_memarg(buffer)

    if data == b"\x3f":
        assert buffer.read(1) == b"\0"
        return "memory.size", data[0]

    if data == b"\x40":
        assert buffer.read(1) == b"\0"
        return "memory.grow", data[0]

    # Numeric instructions
    # https://www.w3.org/TR/wasm-core-1/#numeric-instructions%E2%91%A6
    if data == b"\x41":
        return ("i32.const", data[0]) + expand_bytes(read_sint(buffer, 32))

    if data == b"\x42":
        return ("i64.const", data[0]) + expand_bytes(read_sint(buffer, 64), 64)

    if data == b"\x43":
        return ("f32.const", data[0]) + expand_bytes(read_f32(buffer))

    if data == b"\x44":
        return ("f64.const", data[0]) + expand_bytes(read_f64(buffer))

    if data == b"\x45":
        return "i32.eqz", data[0]

    if data == b"\x46":
        return "i32.eq", data[0]

    if data == b"\x47":
        return "i32.ne", data[0]

    if data == b"\x48":
        return "i32.lt_s", data[0]

    if data == b"\x49":
        return "i32.lt_u", data[0]

    if data == b"\x4a":
        return "i32.gt_s", data[0]

    if data == b"\x4b":
        return "i32.gt_u", data[0]

    if data == b"\x4c":
        return "i32.le_s", data[0]

    if data == b"\x4d":
        return "i32.le_u", data[0]

    if data == b"\x4e":
        return "i32.ge_s", data[0]

    if data == b"\x4f":
        return "i32.ge_u", data[0]

    if data == b"\x50":
        return "i64.eqz", data[0]

    if data == b"\x51":
        return "i64.eq", data[0]

    if data == b"\x52":
        return "i64.ne", data[0]

    if data == b"\x53":
        return "i64.lt_s", data[0]

    if data == b"\x54":
        return "i64.lt_u", data[0]

    if data == b"\x55":
        return "i64.gt_s", data[0]

    if data == b"\x56":
        return "i64.gt_u", data[0]

    if data == b"\x57":
        return "i64.le_s", data[0]

    if data == b"\x58":
        return "i64.le_u", data[0]

    if data == b"\x59":
        return "i64.ge_s", data[0]

    if data == b"\x5a":
        return "i64.ge_u", data[0]

    if data == b"\x5b":
        return "f32.eq", data[0]

    if data == b"\x5c":
        return "f32.ne", data[0]

    if data == b"\x5d":
        return "f32.lt", data[0]

    if data == b"\x5e":
        return "f32.gt", data[0]

    if data == b"\x5f":
        return "f32.le", data[0]

    if data == b"\x60":
        return "f32.ge", data[0]

    if data == b"\x61":
        return "f64.eq", data[0]

    if data == b"\x62":
        return "f64.ne", data[0]

    if data == b"\x63":
        return "f64.lt", data[0]

    if data == b"\x64":
        return "f64.gt", data[0]

    if data == b"\x65":
        return "f64.le", data[0]

    if data == b"\x66":
        return "f64.ge", data[0]

    if data == b"\x67":
        return "i32.clz", data[0]

    if data == b"\x68":
        return "i32.ctz", data[0]

    if data == b"\x69":
        return "i32.popcnt", data[0]

    if data == b"\x6a":
        return "i32.add", data[0]

    if data == b"\x6b":
        return "i32.sub", data[0]

    if data == b"\x6c":
        return "i32.mul", data[0]

    if data == b"\x6d":
        return "i32.div_s", data[0]

    if data == b"\x6e":
        return "i32.div_u", data[0]

    if data == b"\x6f":
        return "i32.rem_s", data[0]

    if data == b"\x70":
        return "i32.rem_u", data[0]

    if data == b"\x71":
        return "i32.and", data[0]

    if data == b"\x72":
        return "i32.or", data[0]

    if data == b"\x73":
        return "i32.xor", data[0]

    if data == b"\x74":
        return "i32.shl", data[0]

    if data == b"\x75":
        return "i32.shr_s", data[0]

    if data == b"\x76":
        return "i32.shr_u", data[0]

    if data == b"\x77":
        return "i32.rotl", data[0]

    if data == b"\x78":
        return "i32.rotr", data[0]

    if data == b"\x79":
        return "i64.clz", data[0]

    if data == b"\x7a":
        return "i64.ctz", data[0]

    if data == b"\x7b":
        return "i64.popcnt", data[0]

    if data == b"\x7c":
        return "i64.add", data[0]

    if data == b"\x7d":
        return "i64.sub", data[0]

    if data == b"\x7e":
        return "i64.mul", data[0]

    if data == b"\x7f":
        return "i64.div_s", data[0]

    if data == b"\x80":
        return "i64.div_u", data[0]

    if data == b"\x81":
        return "i64.rem_s", data[0]

    if data == b"\x82":
        return "i64.rem_u", data[0]

    if data == b"\x83":
        return "i64.and", data[0]

    if data == b"\x84":
        return "i64.or", data[0]

    if data == b"\x85":
        return "i64.xor", data[0]

    if data == b"\x86":
        return "i64.shl", data[0]

    if data == b"\x87":
        return "i64.shr_s", data[0]

    if data == b"\x88":
        return "i64.shr_u", data[0]

    if data == b"\x89":
        return "i64.rotl", data[0]

    if data == b"\x8a":
        return "i64.rotr", data[0]

    if data == b"\x8b":
        return "f32.abs", data[0]

    if data == b"\x8c":
        return "f32.neg", data[0]

    if data == b"\x8d":
        return "f32.ceil", data[0]

    if data == b"\x8e":
        return "f32.floor", data[0]

    if data == b"\x8f":
        return "f32.trunc", data[0]

    if data == b"\x90":
        return "f32.nearset", data[0]

    if data == b"\x91":
        return "f32.sqrt", data[0]

    if data == b"\x92":
        return "f32.add", data[0]

    if data == b"\x93":
        return "f32.sub", data[0]

    if data == b"\x94":
        return "f32.mul", data[0]

    if data == b"\x95":
        return "f32.div", data[0]

    if data == b"\x96":
        return "f32.min", data[0]

    if data == b"\x97":
        return "f32.max", data[0]

    if data == b"\x98":
        return "f32.copysign", data[0]

    if data == b"\x99":
        return "f64.abs", data[0]

    if data == b"\x9a":
        return "f64.neg", data[0]

    if data == b"\x9b":
        return "f64.ceil", data[0]

    if data == b"\x9c":
        return "f64.floor", data[0]

    if data == b"\x9d":
        return "f64.trunc", data[0]

    if data == b"\x9e":
        return "f64.nearset", data[0]

    if data == b"\x9f":
        return "f64.sqrt", data[0]

    if data == b"\xa0":
        return "f64.add", data[0]

    if data == b"\xa1":
        return "f64.sub", data[0]

    if data == b"\xa2":
        return "f64.mul", data[0]

    if data == b"\xa3":
        return "f64.div", data[0]

    if data == b"\xa4":
        return "f64.min", data[0]

    if data == b"\xa5":
        return "f64.max", data[0]

    if data == b"\xa6":
        return "f64.copysign", data[0]

    if data == b"\xa7":
        return "i32.wrap_i64", data[0]

    if data == b"\xa8":
        return "i32.trunc_f32_s", data[0]

    if data == b"\xa9":
        return "i32.trunc_f32_u", data[0]

    if data == b"\xaa":
        return "i32.trunc_f64_s", data[0]

    if data == b"\xab":
        return "i32.trunc_f64_u", data[0]

    if data == b"\xac":
        return "i64.extend_i32_s", data[0]

    if data == b"\xad":
        return "i64.extend_i32_u", data[0]

    if data == b"\xae":
        return "i64.trunc_f32_s", data[0]

    if data == b"\xaf":
        return "i64.trunc_f32_u", data[0]

    if data == b"\xb0":
        return "i64.trunc_f64_s", data[0]

    if data == b"\xb1":
        return "i64.trunc_f64_u", data[0]

    if data == b"\xb2":
        return "f32.convert_i32_s", data[0]

    if data == b"\xb3":
        return "f32.convert_i32_u", data[0]

    if data == b"\xb4":
        return "f32.convert_i64_s", data[0]

    if data == b"\xb5":
        return "f32.convert_i64_u", data[0]

    if data == b"\xb6":
        return "f32.demote_f64", data[0]

    if data == b"\xb7":
        return "f64.convert_i32_s", data[0]

    if data == b"\xb8":
        return "f64.convert_i32_u", data[0]

    if data == b"\xb9":
        return "f64.convert_i64_s", data[0]

    if data == b"\xba":
        return "f64.convert_i64_u", data[0]

    if data == b"\xbb":
        return "f64.promote_f32", data[0]

    if data == b"\xbc":
        return "i32.reinterpret_f32", data[0]

    if data == b"\xbd":
        return "i64.reinterpret_f64", data[0]

    if data == b"\xbe":
        return "f32.reinterpret_i32", data[0]

    if data == b"\xbf":
        return "f64.reinterpret_i64", data[0]

    raise TypeError("Invalid instruction type.")
