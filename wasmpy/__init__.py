import sys

import wasmpy.binary
import wasmpy.text


sys.meta_path.insert(0, wasmpy.text.WebAssemblyTextLoader())
sys.meta_path.insert(0, wasmpy.binary.WebAssemblyBinaryLoader())
__all__ = []
