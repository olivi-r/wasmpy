import json, os, platform, setuptools.command.build_ext, struct, subprocess


opcodes = {}
with open(
    os.path.join(os.path.dirname(__file__), "wasmpy", "opcodes.json")
) as fp:
    data = json.load(fp)
    prefixes = data["prefixes"]
    replacements = data["replacements"]
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
        ),
        ("verbose", "v", "increase output verbosity"),
    ]

    def initialize_options(self):
        self.verbose = None

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

    def log(self, out):
        if self.verbose is not None:
            print(" ".join(out))

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
                    self.log(cmd)
                    subprocess.call(cmd)

                    if platform.system() == "Linux":
                        cmd = arg[1] + [name, name + ".o"]
                        self.log(cmd)
                        subprocess.call(cmd)

                    elif platform.system() == "Windows":
                        cmd = arg[2] + [name + ".tmp", name + ".o"]
                        self.log(cmd)
                        subprocess.call(cmd)
                        cmd = arg[3] + [name + ".tmp", name]
                        self.log(cmd)
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
        if is_x86():
            if struct.calcsize("P") == 4:
                machine = "x86"

            else:
                machine = "x86_64"

        else:
            raise ValueError("Unknown architecture")

        with open(f"wasmpy/arch/{machine}/lib/opcodes.cpp", "w+") as out:
            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.hpp"\n\n',
                    "bytes decodeOperation(bytes buf, size_t offset)\n{\n\t",
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
                    if inst in prefixes:
                        out.write("".join(prefixes[inst]))

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
