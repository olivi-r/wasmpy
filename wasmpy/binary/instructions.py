import wasmpy.binary.types
import wasmpy.binary.values
import wasmpy.util


def read_expr_binary(buffer: object) -> list:
    """Read an expression from buffer."""
    in_ = []
    instruction = read_instruction_binary(buffer)
    while instruction[0] != "end":
        in_ += instruction
        instruction = read_instruction_binary(buffer)

    return in_


def read_instruction_binary(buffer: object) -> list:
    """Read an instruction from buffer."""
    data = buffer.read(1)[0]

    # Control instructions
    if data in (0, 1):
        return [data]

    if data == 0x2:
        try:
            rt = wasmpy.binary.types.read_valtype(buffer)

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
            rt = wasmpy.binary.types.read_valtype(buffer)

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
            rt = wasmpy.binary.types.read_valtype(buffer)

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
        return ["br", wasmpy.binary.values.read_uint(buffer, 32)]

    if data == 0xD:
        return ["br_if", wasmpy.binary.values.read_uint(buffer, 32)]

    if data == 0xE:
        l = [
            wasmpy.binary.values.read_uint(buffer, 32)
            for _ in range(wasmpy.binary.values.get_vec_len(buffer))
        ]
        return ["br_table", l, wasmpy.binary.values.read_uint(buffer, 32)]

    if data == 0xF:
        return ["return"]

    if data == 0x10:
        return ["call", wasmpy.binary.values.read_uint(buffer, 32)]

    if data == 0x11:
        x = wasmpy.binary.values.read_uint(buffer, 32)
        assert buffer.read(1)[0] == 0, "Invalid instruction."
        return ["call_indirect", x]

    # Parametric instructions
    if data in (0x1A, 0x1B):
        return [data]

    # Variable instructions
    if data in range(0x20, 0x25):
        return [data] + wasmpy.util.expand_bytes(
            wasmpy.binary.values.read_uint(buffer, 32)
        )

    # Memory instructions
    if data in range(0x28, 0x3F):
        return (
            [data]
            + wasmpy.util.expand_bytes(
                wasmpy.binary.values.read_uint(buffer, 32)
            )  # align
            + wasmpy.util.expand_bytes(
                wasmpy.binary.values.read_uint(buffer, 32)
            )  # offset
        )

    if data in (0x3F, 0x40):
        assert buffer.read(1)[0] == 0
        return [data]

    # Numeric instructions
    if data == 0x41:
        return [data] + wasmpy.util.expand_bytes(
            wasmpy.binary.values.read_sint(buffer, 32)
        )

    if data == 0x42:
        return [data] + wasmpy.util.expand_bytes(
            wasmpy.binary.values.read_sint(buffer, 64), 64
        )

    if data == 0x43:
        return [data] + wasmpy.util.expand_bytes(
            wasmpy.binary.values.read_f32(buffer)
        )

    if data == 0x44:
        return [data] + wasmpy.util.expand_bytes(
            wasmpy.binary.values.read_f64(buffer)
        )

    if data in range(0x45, 0xC0):
        return [data]

    raise TypeError("Invalid instruction type.")
