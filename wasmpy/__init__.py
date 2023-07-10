from . import hooks
import sys


sys.meta_path.insert(0, hooks.WebAssemblyTextLoader())
sys.meta_path.insert(0, hooks.WebAssemblyBinaryLoader())
