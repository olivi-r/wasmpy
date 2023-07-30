from . import binary, text
import sys


sys.meta_path.insert(0, text.WebAssemblyTextLoader())
sys.meta_path.insert(0, binary.WebAssemblyBinaryLoader())
