import re
import struct

import wasmpy.util


def read_expr_text(source: list) -> list:
    """Read an expression from source."""
    in_ = []
    offset = 0
    while offset < len(source):
        instruction = read_instruction_text(source, offset)
        offset += 1 + instruction.pop()
        in_ += instruction

    return in_


def read_instruction_text(source, offset: int) -> list:
    """Read the instruction from source at offset."""
    data = source[offset]
    if data in wasmpy.util.opcodes:
        op = wasmpy.util.opcodes[data]
        if op not in wasmpy.util.consumes:
            return [op, 0]

        elif op == 0x40:
            assert source[offset + 1] == 0
            return [0x40, 0]

        else:
            immediate = source[offset + 1]
            if op in (0x43, 0x44):
                if isinstance(immediate, (float, int)):
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
                        return [op] + wasmpy.util.expand_bytes(immediate_value) + [1]

                    else:
                        return (
                            [op] + wasmpy.util.expand_bytes(immediate_value, 64) + [1]
                        )

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
                        for i, digit in enumerate(hexfloat.group("q").replace("_", "")):
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
                [op]
                + wasmpy.util.expand_bytes(immediate, wasmpy.util.consumes[op] * 8)
                + [1]
            )
