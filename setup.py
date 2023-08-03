import json, os, platform, setuptools.command.build_ext, struct, subprocess


prefixed = {
    "local.get": (
        "localidx = buf.at(offset + 4) << 24 | buf.at(offset + 3) << 16 | buf.at(offset + 2) << 8 | buf.at(offset + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
        "localidx += 10;\n\t\t\t",
    ),
    "local.set": (
        "localidx = buf.at(offset + 4) << 24 | buf.at(offset + 3) << 16 | buf.at(offset + 2) << 8 | buf.at(offset + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "local.tee": (
        "localidx = buf.at(offset + 4) << 24 | buf.at(offset + 3) << 16 | buf.at(offset + 2) << 8 | buf.at(offset + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "global.get": (
        "ll = buf.at(offset + 4) << 24 | buf.at(offset + 3) << 16 | buf.at(offset + 2) << 8 | buf.at(offset + 1);\n\t\t\t",
        "ll *= 9;\n\t\t\t",
        "ll += (uint64_t)globalTableAddr;\n\t\t\t",
        "lh = ll + 2;\n\t\t\t",
        "hl = ll + 4;\n\t\t\t",
        "hh = ll + 6;\n\t\t\t",
        "bits = ll + 8;\n\t\t\t",
    ),
    "global.set": (
        "ll = buf.at(offset + 4) << 24 | buf.at(offset + 3) << 16 | buf.at(offset + 2) << 8 | buf.at(offset + 1);\n\t\t\t",
        "ll *= 9;\n\t\t\t",
        "ll += (uint64_t)globalTableAddr;\n\t\t\t",
        "lh = ll + 2;\n\t\t\t",
        "hl = ll + 4;\n\t\t\t",
        "hh = ll + 6;\n\t\t\t",
    ),
}

global_32 = "(uint8_t){b}, (uint8_t)({b} >> 8), (uint8_t)({b} >> 16), (uint8_t)({b} >> 24)"
global_64 = (
    global_32
    + ", (uint8_t)({b} >> 32), (uint8_t)({b} >> 40), (uint8_t)({b} >> 48), (uint8_t)({b} >> 56)"
)
error_32 = "(uint8_t)(errorPageAddr + {o}), (uint8_t)((errorPageAddr + {o}) >> 8), (uint8_t)((errorPageAddr + {o}) >> 16), (uint8_t)((errorPageAddr + {o}) >> 24)"
error_64 = (
    error_32
    + ", (uint8_t)((errorPageAddr + {o}) >> 32), (uint8_t)((errorPageAddr + {o}) >> 40), (uint8_t)((errorPageAddr + {o}) >> 48), (uint8_t)((errorPageAddr + {o}) >> 56)"
)

replacements = {
    "ret_v32": (
        (
            "0, 0, 0, 0, 0, 0, 0, 255",
            "(uint8_t)(errorPageAddr + 1), (uint8_t)((errorPageAddr + 1) >> 8), (uint8_t)((errorPageAddr + 1) >> 16), (uint8_t)((errorPageAddr + 1) >> 24), (uint8_t)((errorPageAddr + 1) >> 32), (uint8_t)((errorPageAddr + 1) >> 40), (uint8_t)((errorPageAddr + 1) >> 48), (uint8_t)((errorPageAddr + 1) >> 56)",
        ),
        (
            "255, 0, 0, 0, 0, 0, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24), (uint8_t)(errorPageAddr >> 32), (uint8_t)(errorPageAddr >> 40), (uint8_t)(errorPageAddr >> 48), (uint8_t)(errorPageAddr >> 56)",
        ),
        (
            "0, 0, 0, 255",
            "(uint8_t)(errorPageAddr + 1), (uint8_t)((errorPageAddr + 1) >> 8), (uint8_t)((errorPageAddr + 1) >> 16), (uint8_t)((errorPageAddr + 1) >> 24)",
        ),
        (
            "255, 0, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24)",
        ),
    ),
    "ret_v64": (
        (
            "0, 0, 0, 0, 0, 0, 0, 255",
            "(uint8_t)(errorPageAddr + 1), (uint8_t)((errorPageAddr + 1) >> 8), (uint8_t)((errorPageAddr + 1) >> 16), (uint8_t)((errorPageAddr + 1) >> 24), (uint8_t)((errorPageAddr + 1) >> 32), (uint8_t)((errorPageAddr + 1) >> 40), (uint8_t)((errorPageAddr + 1) >> 48), (uint8_t)((errorPageAddr + 1) >> 56)",
        ),
        (
            "255, 0, 0, 0, 0, 0, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24), (uint8_t)(errorPageAddr >> 32), (uint8_t)(errorPageAddr >> 40), (uint8_t)(errorPageAddr >> 48), (uint8_t)(errorPageAddr >> 56)",
        ),
        (
            "0, 0, 0, 255",
            "(uint8_t)(errorPageAddr + 1), (uint8_t)((errorPageAddr + 1) >> 8), (uint8_t)((errorPageAddr + 1) >> 16), (uint8_t)((errorPageAddr + 1) >> 24)",
        ),
        (
            "255, 0, 0, 255",
            "(uint8_t)(errorPageAddr + 5), (uint8_t)((errorPageAddr + 5) >> 8), (uint8_t)((errorPageAddr + 5) >> 16), (uint8_t)((errorPageAddr + 5) >> 24)",
        ),
        (
            "0, 255, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24)",
        ),
    ),
    "ret_void": (
        (
            "0, 0, 0, 0, 0, 0, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24), (uint8_t)(errorPageAddr >> 32), (uint8_t)(errorPageAddr >> 40), (uint8_t)(errorPageAddr >> 48), (uint8_t)(errorPageAddr >> 56)",
        ),
        (
            "0, 0, 0, 255",
            "(uint8_t)errorPageAddr, (uint8_t)(errorPageAddr >> 8), (uint8_t)(errorPageAddr >> 16), (uint8_t)(errorPageAddr >> 24)",
        ),
    ),
    "unreachable": (
        # 64 bit replacements
        ("0, 0, 0, 0, 0, 0, 0, 255", error_64.format(o=9)),
        # 32 bit replacements
        ("0, 0, 0, 255", error_32.format(o=9)),
    ),
    "local.get": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "local.set": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "local.tee": (
        (
            "255, 0, 255, 0",
            "(uint8_t)localidx, (uint8_t)(localidx >> 8), (uint8_t)(localidx >> 16), (uint8_t)(localidx >> 24)",
        ),
    ),
    "global.get": (
        # 64 bit replacements
        ("0, 0, 0, 255, 0, 0, 0, 0", global_64.format(b="ll")),
        ("255, 0, 0, 255, 0, 0, 0, 0", global_64.format(b="lh")),
        ("0, 255, 0, 255, 0, 0, 0, 0", global_64.format(b="hl")),
        ("255, 255, 0, 255, 0, 0, 0, 0", global_64.format(b="hh")),
        ("255, 255, 255, 255, 0, 0, 0, 0", global_64.format(b="bits")),
        # 32 bit replacements
        ("0, 0, 0, 255", global_32.format(b="ll")),
        ("255, 0, 0, 255", global_32.format(b="lh")),
        ("0, 255, 0, 255", global_32.format(b="hl")),
        ("255, 255, 0, 255", global_32.format(b="hh")),
        ("255, 255, 255, 255", global_32.format(b="bits")),
    ),
    "global.set": (
        # 64 bit replacements
        ("0, 0, 0, 255, 0, 0, 0, 0", global_64.format(b="ll")),
        ("255, 0, 0, 255, 0, 0, 0, 0", global_64.format(b="lh")),
        ("0, 255, 0, 255, 0, 0, 0, 0", global_64.format(b="hl")),
        ("255, 255, 0, 255, 0, 0, 0, 0", global_64.format(b="hh")),
        # 32 bit replacements
        ("0, 0, 0, 255", global_32.format(b="ll")),
        ("255, 0, 0, 255", global_32.format(b="lh")),
        ("0, 255, 0, 255", global_32.format(b="hl")),
        ("255, 255, 0, 255", global_32.format(b="hh")),
    ),
    "i32.const": (
        ("0, 0", "buf.at(offset + 1), buf.at(offset + 2)"),
        ("255, 255", "buf.at(offset + 3), buf.at(offset + 4)"),
    ),
    "i64.const": (
        ("0, 0", "buf.at(offset + 1), buf.at(offset + 2)"),
        ("0, 255", "buf.at(offset + 3), buf.at(offset + 4)"),
        ("255, 0", "buf.at(offset + 5), buf.at(offset + 6)"),
        ("255, 255", "buf.at(offset + 7), buf.at(offset + 8)"),
    ),
    "f32.const": (
        ("0, 0", "buf.at(offset + 1), buf.at(offset + 2)"),
        ("255, 255", "buf.at(offset + 3), buf.at(offset + 4)"),
    ),
    "f64.const": (
        ("0, 0", "buf.at(offset + 1), buf.at(offset + 2)"),
        ("0, 255", "buf.at(offset + 3), buf.at(offset + 4)"),
        ("255, 0", "buf.at(offset + 5), buf.at(offset + 6)"),
        ("255, 255", "buf.at(offset + 7), buf.at(offset + 8)"),
    ),
    "i32.div_s": (
        # 64 bit replacements
        ("0, 0, 0, 0, 0, 0, 0, 255", error_64.format(o=10)),
        ("255, 0, 0, 0, 0, 0, 0, 255", error_64.format(o=11)),
        # 32 bit replacements
        ("0, 0, 0, 255", error_32.format(o=10)),
        ("255, 0, 0, 255", error_32.format(o=11)),
    ),
    "i32.div_u": (
        # 64 bit replacements
        ("0, 0, 0, 0, 0, 0, 0, 255", error_64.format(o=10)),
        # 32 bit replacements
        ("0, 0, 0, 255", error_32.format(o=10)),
    ),
}


def listdir(path: str) -> list:
    return [os.path.join(path, p) for p in os.listdir(path)]


def is_x86() -> bool:
    return platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64")


class assemble(setuptools.Command):
    user_options = [
        (
            "targets=",
            "T",
            "comma separated list of targets to assemble, defaults to current "
            "Python's architecture, e.g. --targets=x86_64,...",
        )
    ]

    def initialize_options(self):
        if is_x86():
            if struct.calcsize("P") == 4:
                self.targets = "x86"

            else:
                self.targets = "x86_64"

        else:
            raise ValueError("Unknown architecture")

    def finalize_options(self):
        self.targets = self.targets.split(",")
        for target in self.targets:
            if target not in os.listdir("wasmpy/arch") or os.path.isfile(
                f"wasmpy/{target}"
            ):
                raise ValueError(f"Unknown architecture: {target}")

    def run(self):
        args = {
            "x86_64": [
                ["as", "-o"],
                ["ld", "--oformat", "binary", "-o"],
                ["ld", "-T", "NUL", "--image-base", "0", "-o"],
                ["objcopy", "-O", "binary", "-j", ".text"],
            ]
        }

        args["x86"] = [i.copy() for i in args["x86_64"]]
        args["x86"][0].insert(1, "--32")
        args["x86"][1].insert(1, "-melf_i386")
        args["x86"][2].insert(1, "-mi386pe")

        for target in self.targets:
            arg = args[target]
            for source in listdir(f"wasmpy/arch/{target}"):
                if os.path.isdir(source):
                    continue

                if os.path.splitext(source)[1].lower() == ".s":
                    name = os.path.splitext(source)[0]
                    cmd = arg[0] + [name + ".o", source]
                    subprocess.call(cmd)

                    if platform.system() == "Linux":
                        cmd = arg[1] + [name, name + ".o"]
                        subprocess.call(cmd)

                    elif platform.system() == "Windows":
                        cmd = arg[2] + [name + ".tmp", name + ".o"]
                        subprocess.call(cmd)
                        cmd = arg[3] + [name + ".tmp", name]
                        subprocess.call(cmd)


class tidy(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        for machine in os.listdir("wasmpy/arch"):
            if os.path.isfile("wasmpy/arch/" + machine):
                continue

            if os.path.exists(f"wasmpy/arch/{machine}/lib/opcodes.cpp"):
                os.remove(f"wasmpy/arch/{machine}/lib/opcodes.cpp")

            for file in listdir(f"wasmpy/arch/{machine}"):
                if os.path.isdir(file):
                    continue

                if os.path.splitext(file)[1] != ".s":
                    os.remove(file)


class gen_opcodes(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        # generate opcodes.cpp
        opcodes = {}
        with open(
            os.path.join(os.path.dirname(__file__), "wasmpy", "opcodes.json")
        ) as fp:
            data = json.load(fp)
            for group in data["opcodes"]:
                opcodes.update(
                    dict(
                        zip(
                            (i[0] for i in group["instructions"]),
                            (
                                i + group["offset"]
                                for i in range(len(group["instructions"]))
                            ),
                        )
                    )
                )

        if struct.calcsize("P") == 4:
            machine = "x86"

        else:
            machine = "x86_64"

        with open(f"wasmpy/arch/{machine}/lib/opcodes.cpp", "w+") as out:
            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.hpp"\n\n',
                    "bytes decodeOperation(bytes buf, size_t offset, char plat)\n{\n\t",
                    "bytes insts = {};\n\t",
                    "int localidx;\n\t",
                    "uint64_t hh, hl, lh, ll, bits;\n\t"
                    "switch (buf.at(offset))\n\t{\n\t",
                )
            )

            for file in listdir(f"wasmpy/arch/{machine}"):
                if os.path.isdir(file):
                    continue

                inst = os.path.basename(file)

                if inst in opcodes:
                    with open(file, "rb") as fp:
                        data = ", ".join(str(i) for i in fp.read() if i != 0x90)

                    # apply replacements
                    if inst in replacements:
                        for replacement in replacements[inst]:
                            data = data.replace(*replacement)

                    out.write(f"case {opcodes[inst]}:\n\t\t")
                    if inst in prefixed:
                        out.write("".join(prefixed[inst]))

                    out.write(f"insts = {{{data}}};\n\t\t")

                    out.write("break;\n\n\t")

            out.write("default:\n\t\tbreak;\n\t}\n\treturn insts;\n}\n\n")

            for file in listdir(f"wasmpy/arch/{machine}"):
                if os.path.isdir(file):
                    continue

                name = os.path.basename(file)
                if name not in opcodes:
                    with open(file, "rb") as fp:
                        data = ", ".join(str(i) for i in fp.read() if i != 0x90)

                    if os.path.splitext(name)[1] == "":
                        # apply replacements
                        if name in replacements:
                            for replacement in replacements[name]:
                                data = data.replace(*replacement)

                        if name in ("ret_v32", "ret_v64", "ret_void"):
                            out.write(
                                f"\nbytes {name}(uint64_t errorPageAddr)\n{{\n\treturn {{{data}}};\n}}\n"
                            )

                        else:
                            out.write(f"bytes {name} = {{{data}}};\n")


class build_ext(setuptools.command.build_ext.build_ext):
    def run(self):
        self.run_command("gen_opcodes")
        setuptools.command.build_ext.build_ext.run(self)


ext = [
    setuptools.Extension(
        "wasmpy.wasi_unstable",
        sources=["wasmpy/wasi.c"],
        define_macros=[("WASI_UNSTABLE", None)],
        py_limited_api=True,
    ),
    setuptools.Extension(
        "wasmpy.wasi_snapshot_preview1",
        sources=["wasmpy/wasi.c"],
        py_limited_api=True,
    ),
]

if is_x86():
    if struct.calcsize("P") == 4:
        machine = "x86"

    else:
        machine = "x86_64"

    ext.append(
        setuptools.Extension(
            "wasmpy.nativelib",
            sources=[
                f"wasmpy/arch/{machine}/lib/lib.cpp",
                f"wasmpy/arch/{machine}/lib/opcodes.cpp",
            ],
            py_limited_api=True,
        )
    )


with open("README.md", "r") as fp:
    description = fp.read()

setuptools.setup(
    name="wasmpy",
    version="0.1.4",
    author="Olivia Ryan",
    author_email="olivia.r.dev@gmail.com",
    description="WebAssembly from Python.",
    long_description=description,
    long_description_content_type="text/markdown",
    url="https://github.com/olivi-r/wasmpy",
    packages=["wasmpy", "wasmpy.binary", "wasmpy.text"],
    package_data={"wasmpy": ["opcodes.json"]},
    ext_modules=ext,
    options={"bdist_wheel": {"py_limited_api": "cp36"}},
    cmdclass={
        "assemble": assemble,
        "tidy": tidy,
        "build_ext": build_ext,
        "gen_opcodes": gen_opcodes,
    },
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Environment :: WebAssembly",
        "Programming Language :: C++",
        "Programming Language :: Assembly",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "License :: OSI Approved :: MIT License",
    ],
    license="MIT",
    python_requires=">=3.6",
    install_requires=["sexpdata"],
)
