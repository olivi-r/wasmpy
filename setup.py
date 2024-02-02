import glob
import json
import os
import platform
import struct

import setuptools.command.build_ext


opcodes = {}
with open(os.path.join(os.path.dirname(__file__), "wasmpy/opcodes.json")) as fp:
    data = json.load(fp)
    replacements = data["replacements"]
    functions = data["functions"]
    for group in data["opcodes"]:
        opcodes.update(
            dict(
                zip(
                    (i[0] for i in group["instructions"]),
                    (i + group["offset"] for i in range(len(group["instructions"]))),
                )
            )
        )


def is_x86() -> bool:
    return platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64")


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
                    "bool decodeOperation(bytes buf, size_t offset, bytes *insts)\n{\n\t",
                    "switch (buf.at(offset))\n\t{\n\t",
                )
            )

            for file in glob.glob(f"wasmpy/arch/{machine}/**/*"):
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

                    out.write(f"*insts = {{{data}}};\n\t\t")

                    out.write("break;\n\n\t")

            out.write(
                'default:\n\t\tPyErr_SetString(PyExc_NotImplementedError, "unimplemented instruction");\n\t\treturn false;\n\t}\n\treturn true;\n}\n'
            )

            for file in glob.glob(f"wasmpy/arch/{machine}/*"):
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

                        if name in functions:
                            out.write(
                                f"\nbytes {name}({functions[name]})\n{{\n\treturn {{{data}}};\n}}\n"
                            )

                        else:
                            out.write(f"\nbytes {name} = {{{data}}};\n")


class build_ext(setuptools.command.build_ext.build_ext):
    def run(self):
        self.run_command("gen_opcodes")
        setuptools.command.build_ext.build_ext.run(self)


plat = []
if platform.system() == "Linux":
    plat = [("PLATFORM_LINUX", None)]

elif platform.system() == "Windows":
    plat = [("PLATFORM_WINDOWS", None)]

arch = []
if is_x86():
    if struct.calcsize("P") == 4:
        machine = "x86"
        arch = [("ARCH_X86", None)]

    else:
        machine = "x86_64"
        arch = [("ARCH_X86_64", None)]

ext = [
    setuptools.Extension(
        "wasmpy.nativelib",
        include_dirs=[
            os.path.abspath("wasmpy/include"),
            os.path.abspath(f"wasmpy/arch/{machine}/lib"),
        ],
        sources=[
            f"wasmpy/arch/{machine}/lib/lib.cpp",
            f"wasmpy/arch/{machine}/lib/opcodes.cpp",
            "wasmpy/globals.cpp",
            "wasmpy/memories.cpp",
            "wasmpy/nativelib.cpp",
        ],
        define_macros=plat + arch,
        py_limited_api=True,
    ),
    setuptools.Extension(
        "wasmpy.spectest",
        sources=["wasmpy/spectest.cpp"],
        py_limited_api=True,
    ),
]


with open("README.md", "r") as fp:
    description = fp.read()

setuptools.setup(
    name="wasmpy",
    author="Olivia Ryan",
    author_email="olivia.r.dev@gmail.com",
    description="WebAssembly in Python.",
    long_description=description,
    long_description_content_type="text/markdown",
    url="https://github.com/olivi-r/wasmpy",
    packages=["wasmpy", "wasmpy.binary", "wasmpy.text"],
    package_data={"wasmpy": ["opcodes.json"]},
    ext_modules=ext,
    options={"bdist_wheel": {"py_limited_api": "cp36"}},
    cmdclass={
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
    install_requires=[
        "sexpdata>=1.0.1;python_version>='3.7'",
        "sexpdata==1.0.0;python_version=='3.6'",
    ],
)
