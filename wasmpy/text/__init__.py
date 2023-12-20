import os
import sys

import wasmpy.text.module


class WebAssemblyTextLoader:
    def find_module(self, fullname, path=None):
        fname = fullname.split(".")[-1] + ".wat"
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

        with open(self.fname, "r") as fp:
            mod = wasmpy.text.module.read_module(fp)
            mod.__file__ = self.fname
            mod.__name__ = fullname
            mod.__loader__ = self

        sys.modules[fullname] = mod
        return mod
