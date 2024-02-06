# Building From Source

## Cloning

First either download and extract [Wasmpy's source](https://github.com/olivi-r/wasmpy/archive/refs/heads/main.zip) or clone the repo:

```sh
git clone https://github.com/olivi-r/wasmpy.git
cd wasmpy
```

## Building Assembly Sources

Wasmpy converts WebAssembly to native machine code for the best possible execution speeds.
To faciliate this it has assembly files for the supported architectures that can be built like this:

<!-- tabs:start -->

# **Linux**

```sh
make -f linux.mk
```

# **Windows**

This requires [MinGW-w64](https://winlibs.com/#download-release) to be on PATH

```cmd
mingw32-make -f windows.mk
```

<!-- tabs:end -->

## Installing from source

Once you have built the assembly sources, Wasmpy can be installed simply by calling:

```sh
pip install .
```

## Make Commands

Multiple other make commands are defined for more specifc use cases

### all (default)

Builds assembly sources for all target architectures.

### clean

Deletes all files created by make commands.

### x86

Build 32 bit x86 assembly sources.

### x86_64

Build 64 bit x86 assembly sources.
