from .module import read_module


def load_binary(filepath):
    with open(filepath, "rb") as fp:
        mod = read_module(fp)

    return mod
