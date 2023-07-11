from . import opcodes, types, values
import re, struct


def expand_bytes(n: int, bits: int = 32) -> list:
    """Convert integer to little endian byte representation."""
    r = []
    for _ in range(bits // 8):
        r.append(n & 255)
        n >>= 8

    return r


def read_expr_binary(buffer: object) -> list:
    """Read an expression from buffer."""
    in_ = []
    instruction = read_instruction_binary(buffer)
    while instruction[0] != "end":
        in_ += instruction
        instruction = read_instruction_binary(buffer)

    return in_


def read_expr_text(source: list) -> list:
    in_ = []
    offset = 0
    while offset < len(source):
        instruction = read_instruction_text(source, offset)
        offset += 1 + instruction.pop()
        in_ += instruction

    return in_


def read_instruction_binary(buffer: object) -> list:
    """Read an instruction from buffer."""
    data = buffer.read(1)[0]

    # Control instructions
    if data in (0, 1):
        return [data]

    if data == 0x2:
        try:
            rt = types.read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in_ = []
        instruction = read_instruction_binary(buffer)
        while instruction != ["end"]:
            in_.append(instruction)
            instruction = read_instruction_binary(buffer)

        return ["block", rt, tuple(in_), "end"]

    if data == 0x3:
        try:
            rt = types.read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in_ = []
        instruction = read_instruction_binary(buffer)
        while instruction != "end":
            in_.append(instruction)
            instruction = read_instruction_binary(buffer)

        return ["loop", rt, tuple(in_), "end"]

    if data == 0x4:
        try:
            rt = types.read_valtype(buffer)

        except TypeError:
            buffer.seek(-1, 1)
            if buffer.read(1)[0] != 64:
                raise TypeError("Invalid blocktype.")

            rt = None

        in1 = []
        instruction = read_instruction_binary(buffer)
        while instruction[0] not in ["end", "else"]:
            in1.append(instruction)
            instruction = read_instruction_binary(buffer)

        in1 = tuple(in1)
        if instruction[0] == "end":
            return ["if", rt, in1, "else", (), "end"]

        # "else"
        in2 = []
        instruction = read_instruction_binary(buffer)
        while instruction[0] != "end":
            in2.append(instruction)
            instruction = read_instruction_binary(buffer)

        return ["if", rt, in1, "else", tuple(in2), "end"]

    if data == 0x5:
        return ["else"]

    if data == 0xB:
        return ["end"]

    if data == 0xC:
        return ["br", values.read_uint(buffer, 32)]

    if data == 0xD:
        return ["br_if", values.read_uint(buffer, 32)]

    if data == 0xE:
        l = [
            values.read_uint(buffer, 32)
            for _ in range(values.get_vec_len(buffer))
        ]
        return ["br_table", l, values.read_uint(buffer, 32)]

    if data == 0xF:
        return ["return"]

    if data == 0x10:
        return ["call", values.read_uint(buffer, 32)]

    if data == 0x11:
        x = values.read_uint(buffer, 32)
        assert buffer.read(1)[0] == 0, "Invalid instruction."
        return ["call_indirect", x]

    # Parametric instructions
    if data in (0x1A, 0x1B):
        return [data]

    # Variable instructions
    if data in range(0x20, 0x25):
        return [data] + expand_bytes(values.read_uint(buffer, 32))

    # Memory instructions
    if data in range(0x28, 0x3F):
        return (
            [data]
            + expand_bytes(values.read_uint(buffer, 32))  # align
            + expand_bytes(values.read_uint(buffer, 32))  # offset
        )

    if data in (0x3F, 0x40):
        assert buffer.read(1)[0] == 0
        return [data]

    # Numeric instructions
    if data == 0x41:
        return [data] + expand_bytes(values.read_sint(buffer, 32))

    if data == 0x42:
        return [data] + expand_bytes(values.read_sint(buffer, 64), 64)

    if data == 0x43:
        return [data] + expand_bytes(values.read_f32(buffer))

    if data == 0x44:
        return [data] + expand_bytes(values.read_f64(buffer))

    if data in range(0x45, 0xC0):
        return [data]

    raise TypeError("Invalid instruction type.")


def read_instruction_text(source, offset: int) -> list:
    data = source[offset]
    if data in opcodes.opcodes:
        op = opcodes.opcodes[data]
        if op not in opcodes.consumes:
            return [op, 0]

        else:
            immediate = source[offset + 1]
            if op in (0x43, 0x44):
                if isinstance(immediate, float):
                    # 32 bit floating point immediate
                    if op == 0x43:
                        return [op] + list(struct.pack("<f", immediate)) + [1]

                    else:
                        return [op] + list(struct.pack("<d", immediate)) + [1]

                nan = re.match(
                    r"(?P<sign>[+-]?)nan:0x(?P<n>(([0-9a-fA-F]_?)+[0-9a-fA-F]|[0-9a-fA-F]))",
                    immediate,
                )
                if nan is not None:
                    significand = int(nan.group("n"), 16)
                    if op == 0x43:
                        # 32 bit nan
                        if nan.group("sign") not in "+":
                            immediate_value = 0xFF800000

                        else:
                            immediate_value = 0x7F800000

                        assert significand in range(
                            1, 2**23
                        ), "nan significand out of range"

                    else:
                        # 64 bit nan
                        if nan.group("sign") not in "+":
                            immediate_value = 0xFFF0000000000000

                        else:
                            immediate_value = 0x7FF0000000000000

                        assert significand in range(
                            1, 2**52
                        ), "nan significand out of range"

                    immediate_value |= int(nan.group("n"), 16)
                    if op == 0x43:
                        return [op] + expand_bytes(immediate_value) + [1]

                    else:
                        return [op] + expand_bytes(immediate_value, 64) + [1]

                else:
                    hexfloat = re.match(
                        r"(?P<sign>[+-]?)0x(?P<p>(([0-9a-fA-F]_?)+[0-9a-fA-F]|[0-9a-fA-F]))(\.(?P<q>(([0-9a-fA-F]+_?)+[0-9a-fA-F]|[0-9a-fA-F])?))?([pP](?P<e>[+-]?(([0-9]+_?)+[0-9]|[0-9])))?$",
                        immediate,
                    )
                    if hexfloat is not None:
                        sign = 1
                        if hexfloat.group("sign") not in "+":
                            sign = -1

                        value = int(hexfloat.group("p"), 16)
                        for i, digit in enumerate(
                            hexfloat.group("q").replace("_", "")
                        ):
                            value += int(digit, 16) / (16**i)

                        if hexfloat.group("e"):
                            value *= 2 ** int(hexfloat.group("e"))

                        if op == 0x43:
                            return [op] + list(struct.pack("<f", value)) + [1]

                        else:
                            return [op] + list(struct.pack("<d", value)) + [1]

                    else:
                        raise ValueError(
                            f"unrecognized floating point value: {immediate}"
                        )

            sign = 1
            if isinstance(immediate, str) and immediate[0] in ("+", "-"):
                sign, *immediate = immediate
                immediate = "".join(immediate)
                if sign == "-":
                    sign = -1

            if isinstance(immediate, str) and immediate.startswith("0x"):
                immediate = sign * int(immediate[2:], 16)

            return (
                [op] + expand_bytes(immediate, opcodes.consumes[op] * 8) + [1]
            )
