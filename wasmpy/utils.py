import io, math, struct

f32 = struct.Struct("<f")
f64 = struct.Struct("<d")


def read_f32(buffer):
    return f32.unpack(buffer.read(4))


def read_f64(buffer):
    return f64.unpack(buffer.read(8))


def get_vec_len(buffer):
    """Return the length of a vector consumed from buffer."""
    return read_uint(buffer, 32)


def read_uint(buffer, bits):
    """Read an unsigned integer stored in LEB128 format."""
    consumed_bytes = 0
    result = 0
    shift = 0

    while True:
        byte = buffer.read(1)[0]
        consumed_bytes += 1
        assert consumed_bytes <= math.ceil(bits / 7), "Invalid integer"
        result |= (byte & 127) << shift
        if not byte >> 7:
            break

        shift += 7

    return result


def read_sint(buffer, bits):
    """Read a signed integer stored in LEB128 format."""
    shift = 0
    byte = buffer.read(1)[0]
    result = (byte & 127) << shift
    consumed_bytes = 1

    while (byte := buffer.read(1)[0]) >> 7:
        consumed_bytes += 1
        assert consumed_bytes < math.ceil(bits / 7), "Invalid integer"
        result |= (byte & 127) << shift
        shift += 7

    if shift < bits and (byte >> 6) & 1:
        result |= ~0 << shift

    return result


def read_name(buffer):
    """Read a name from buffer."""
    return buffer.read(get_vec_len(buffer)).decode("utf-8")
