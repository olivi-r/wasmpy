from .module import read_module
import os, sys, types

class WebAssemblyBinaryLoader(object):
    """WebAssembly binary import hook.
    This hook is registered automatically with `import wasmpy`.
    After the initial import a WebAssembly binary format file (.wasm) can
    be loaded with the import statement eg:
    |- mymodule
    |  |- mymodule_2.wasm
    |
    |- mymodule_1.wasm
    |- example.py
    in example.py:
    ```
    import wasmpy
    import mymodule_1
    from mymodule import mymodule_2
    ``` Will load both binary files.
    """
    def find_module(self, fullname, path=None):
        fname = fullname.split(".")[-1] + ".wasm"
        if path is not None and len(path):
            for p in path:
                if os.path.isfile(os.path.join(p, fname)):
                    self.fname = os.path.join(p, fname)
                    return self

        elif len(fullname.split(".")) < 2 and os.path.isfile(fname):
            self.fname = os.path.abspath(fname)
            return self

    def load_module(self, fullname):
        if fullname in sys.modules:
            return

        mod = types.ModuleType(fullname)
        mod.__file__ = self.fname
        mod.__name__ = fullname
        mod.__loader__ = self
        sys.modules[fullname] = mod
        with open(self.fname, "rb") as fp:
            mod.module = read_module(fp)

        return mod
