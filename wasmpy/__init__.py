from .hooks import WebAssemblyBinaryLoader
import sys

sys.meta_path.insert(0, WebAssemblyBinaryLoader())
