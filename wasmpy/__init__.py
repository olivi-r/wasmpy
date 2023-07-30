from . import binary, text
import sys as _sys


_sys.meta_path.insert(0, text.WebAssemblyTextLoader())
_sys.meta_path.insert(0, binary.WebAssemblyBinaryLoader())
