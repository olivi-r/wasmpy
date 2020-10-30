from .module import read_module


def load_binary(filepath):
    """Load a WebAssembly binary from file."""
    with open(filepath, "rb") as fp:
        mod = read_module(fp)

    return mod
