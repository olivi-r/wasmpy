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

## Usage with Python modules

```
|-  my_module
    |-  __init__.py
    |-  wasm_math.wat
```

```python
# __init__.py

import wasmpy
from .wasm_math import add
```

```webassembly
;; wasm_math.wat

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

## Invalid function names

Functions exported from WebAssembly can also be accessed from the module by using their name as a key.

For exported names that aren't valid Python identifiers or which start with a `_`, this is the only valid way of accessing these functions.

```webassembly
;; wasm_math.wat

(module
    (func (export "add one") (param i32) (result i32)
        (i32.add (local.get 0) (i32.const 1))
    )
)
```

```python
>>> import wasmpy
>>> import wasm_math
>>> wasm_math["add one"](11)
12
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

# Goals

- Current target:

  - Reach compatability with the [MVP](https://www.w3.org/TR/wasm-core-1)

- Future Goals
  - Native support for more architectures, particularly those supported by [manylinux](https://github.com/pypa/manylinux)
  - Interfacing with the [Python C API](https://docs.python.org/3/c-api) from WebAssembly (in conjunction with the Wasmpy sister project [wasmpy-build](https://github.com/olivi-r/wasmpy-build)) to allow compiled extension modules that are platform independent
  - Compatibility with [Pyodide](https://github.com/pyodide/pyodide)
  - Python implementation support for Jython, PyPy etc.
  - Support for [WebAssembly proposals](https://github.com/WebAssembly/proposals)
  - Support for [WASI](https://wasi.dev) snapshots, as well as support for supersets such as [WASIX](https://wasix.org)
