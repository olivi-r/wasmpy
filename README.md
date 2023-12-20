[![AppVeyor Build status](https://ci.appveyor.com/api/projects/status/6jo7yag38m5ilv6h?svg=true)](https://ci.appveyor.com/project/olivi-r/wasmpy)

# wasmpy
WebAssembly in Python.

Wasmpy is a lightweight layer that sits between Python and WebAssembly. When attempting to import a WebAssembly file, the file is converted into machine code for native speeds.

# Installing
```sh
python3 -m pip install wasmpy
```

# Usage
To get started, first import the `wasmpy` module to register the WebAssembly import hooks.

Then you can just `import wasm_or_wat_file` to load a WebAssembly module.

## Example
```
|-  my_module
    |-  __init__.py
    |-  wasm_math.wat
```

File: `__init__.py`
```python
import wasmpy
from .wasm_math import add
```
File: `wasm_math.wat`
```webassembly
(module
    (func (export "add") (param i32 i32) (result i32)
        (i32.add (local.get 0) (local.get 1))
    )
)
```
```python
>>> import my_module
>>> my_module.add(45, 960)
1005
>>>
```

# Building From Source
On Windows this requires the [mingw-w64](https://www.mingw-w64.org/downloads/) `as` tool to be on PATH.
```sh
git clone https://github.com/olivi-r/wasmpy.git
cd wasmpy
python3 setup.py assemble
python3 -m pip install .
```

# Limitations
Wasmpy is still in active development, and only supports x86/x86-64 Windows and Linux machines and lacks some key features:
- most memory instructions
- most control instructions
- imports
- tables

Wasmpy is being developed in accordance with the [wasm-core-1](https://w3.org/TR/wasm-core-1) recommendation, which means it lacks support for features introduced in [WebAssembly 2.0](https://webassembly.github.io/spec/core) drafts.

In future all of these limitations will be overcome and wasmpy will offer support for [WASI](https://wasi.dev) and allow interfacing directly with CPython's [c-api](https://docs.python.org/3/c-api) similar to standard C/C++ Python extensions.
