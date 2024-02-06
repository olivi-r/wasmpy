# Quickstart

## Installation

Wasmpy's latest release is most easily installed from PyPi using pip:

```sh
pip install wasmpy
```

Or, to install a specific version:

```sh
pip install wasmpy==0.2.3
```

## Usage

Wasmpy needs to be imported into your module before any WebAssembly modules are

```python
import wasmpy
import my_webassembly_module
```

Then the module can be used as if it were a regular Python module.

```python
my_webassembly_module.my_command()
```
