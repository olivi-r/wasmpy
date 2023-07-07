[![AppVeyor](https://img.shields.io/appveyor/build/olivi-r/wasmpy)](https://ci.appveyor.com/project/olivi-r/wasmpy)

# wasmpy
WebAssembly from Python.

Wasmpy is a lightweight layer that sits between Python and WebAssembly. When attempting to import a WebAssembly file, the file is converted into machine code for native speeds.

## Installing
Install the latest version:

```sh
python -m pip install wasmpy
```

Or build and install from source:

```sh
git clone https://github.com/olivi-r/wasmpy.git
cd wasmpy
python setup.py assemble
python -m pip install .
```

## Usage
To get started, first import the `wasmpy` module to register the WebAssembly import hooks.

Then you can just run `import wasm_or_wat_file` to load a WebAssembly module.

## Example
file: `wasm_math.wat`
```webassembly
(module
    (func (export "add") (param i32 i32) (result i32)
        (i32.add (local.get 0) (local.get 1))
    )
)
```

```python
>>> import wasmpy
>>> import wasm_math
>>> wasm_math.add(45, 960)
1005
>>>
```

## Limitations
Wasmpy is still in active development, and only supports x86/x86-64 Windows and Linux machines and lacks some key features:
- floating point operations
- control instructions
- memories
- tables
- imports / exports (partial support)

The translated machine code is also naively generated and may not be the most optimized.

In future all of these limitations will be overcome and wasmpy will also offer support for [WASI](https://wasi.dev) and allow interfacing directly with CPython's [c-api](https://docs.python.org/3/c-api/index.html) similar to standard C/C++ Python extensions.
