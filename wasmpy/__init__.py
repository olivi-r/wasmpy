from .hooks import WebAssemblyBinaryLoader, WebAssemblyTextLoader
import sys

sys.meta_path.insert(0, WebAssemblyTextLoader())
sys.meta_path.insert(0, WebAssemblyBinaryLoader())
