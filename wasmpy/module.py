import types


def create_module(module: dict) -> object:
    WebAssemblyModule = type("WebAssemblyModule", (types.ModuleType,), {})

    if module["start"] is not None:
        setattr(WebAssemblyModule, "__call__", module["start"])
        obj = WebAssemblyModule("module")
        setattr(
            obj,
            "__doc__",
            module["start"].__doc__,
        )

    else:
        obj = WebAssemblyModule("module")

    for e in module["exports"]:
        setattr(obj, e["name"], e["obj"])

    if hasattr(WebAssemblyModule, "__call__"):
        # run start function
        obj()

    return obj
