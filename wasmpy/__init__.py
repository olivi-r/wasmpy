from .hooks import WebAssemblyBinaryLoader
import sys

sys.meta_path.append(WebAssemblyBinaryLoader())
