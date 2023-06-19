# WasmPy
Interacting with WebAssembly code from python.

Wasmpy is a fairly lightweight layer that sits between Python and the WebAssembly code. When attempting to import a WebAssembly file, the file is read and it is converted into native machine code for native speeds.

This project is intended to be used in conjunction with [wasmpy-build](https://github.com/olivi-r/wasmpy-build), although it does support regular WebAssembly files too.

## Installing

Install the latest version:

```
$ python -m pip install wasmpy
```

Or build and install from source:

```
$ git clone https://github.com/olivi-r/wasmpy.git
$ cd wasmpy
$ python setup.py install
```

# Usage
WasmPy defines import hooks to make the loading of WebAssembly binary files much easier! Just import the `WasmPy` library then you are good to go!
### Example:
If you have the following project setup:

```
|- my_wasm_file.wasm
|- main.py
```
Then in `main.py` the following code will load the WebAssembly file:
```py
import wasmpy
import my_wasm_file
```
The hook also allows importing the files from submodules, eg:
```
|- main.py
|- my_module
|  |- my_wasm_file.wasm
```
Then 
```py
import wasmpy
from my_module import my_wasm_file
```

Functions can be called with the call function from the imported module:

```py
import wasmpy
import wasm_math

wasm_math.call("add")(...)
```

This is due to WebAssembly supporting exported names that may not be valid Python names, such as `add two numbers`
