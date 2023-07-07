import types


def create_module(module: dict) -> object:
    WebAssemblyModule = type("WebAssemblyModule", (types.ModuleType,), {})

    for e in module["exports"]:
        if isinstance(e["obj"], list):

            def get(self, e=e):
                return e["obj"][1][0]

            prop = property(get)

            if e["obj"][0] == "mut":

                def set(self, val, e=e):
                    e["obj"][1][0] = val

                prop = prop.setter(set)

            setattr(WebAssemblyModule, e["name"], prop)

        else:
            setattr(WebAssemblyModule, e["name"], e["obj"])

    if module["start"] is not None:
        setattr(WebAssemblyModule, "__call__", module["start"])
        obj = WebAssemblyModule("module")
        obj()

    else:
        obj = WebAssemblyModule("module")

    return obj
