import sys


class Trap(Exception):
    pass


import wasmpy.binary
import wasmpy.text
from wasmpy.util import *


sys.meta_path.insert(0, wasmpy.text.WebAssemblyTextLoader())
sys.meta_path.insert(0, wasmpy.binary.WebAssemblyBinaryLoader())
__all__ = wasmpy.util.__all__
