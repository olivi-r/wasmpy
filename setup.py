import setuptools.command.build_ext
import subprocess
import platform
import struct
import json
import os


prefixed = {
    "local.get": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
        "localidx += 10;\n\t\t\t",
    ),
    "local.set": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "local.tee": (
        "localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "localidx *= 10;\n\t\t\t",
    ),
    "global.get": (
        "ll = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "ll *= 9;\n\t\t\t",
        "ll += globalTableAddr;\n\t\t\t",
        "lh = ll + 2;\n\t\t\t",
        "hl = ll + 4;\n\t\t\t",
        "hh = ll + 6;\n\t\t\t",
        "bits = ll + 8;\n\t\t\t",
    ),
    "global.set": (
        "ll = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t\t",
        "ll *= 9;\n\t\t\t",
        "ll += globalTableAddr;\n\t\t\t",
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


replacements = {
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
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("255, 255", "buf.at(i + 3), buf.at(i + 4)"),
    ),
    "i64.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("0, 255", "buf.at(i + 3), buf.at(i + 4)"),
        ("255, 0", "buf.at(i + 5), buf.at(i + 6)"),
        ("255, 255", "buf.at(i + 7), buf.at(i + 8)"),
    ),
    "f32.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("255, 255", "buf.at(i + 3), buf.at(i + 4)"),
    ),
    "f64.const": (
        ("0, 0", "buf.at(i + 1), buf.at(i + 2)"),
        ("0, 255", "buf.at(i + 3), buf.at(i + 4)"),
        ("255, 0", "buf.at(i + 5), buf.at(i + 6)"),
        ("255, 255", "buf.at(i + 7), buf.at(i + 8)"),
    ),
}


def listdir(path):
    return [os.path.join(path, p) for p in os.listdir(path)]


class assemble(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        for source in listdir("wasmpy/x86") + listdir("wasmpy/x86/64"):
            if os.path.isdir(source):
                continue

            # assemble x86 instructions
            if os.path.splitext(source)[1].lower() == ".s":
                subprocess.call(
                    [
                        "as",
                        source,
                        "-o",
                        os.path.splitext(source)[0] + ".o",
                    ]
                )

                if platform.system() == "Linux":
                    subprocess.call(
                        [
                            "ld",
                            "--oformat",
                            "binary",
                            os.path.splitext(source)[0] + ".o",
                            "-o",
                            os.path.splitext(source)[0],
                        ]
                    )

                elif platform.system() == "Windows":
                    subprocess.call(
                        [
                            "ld",
                            "-T",
                            "NUL",
                            "--image-base",
                            "0",
                            os.path.splitext(source)[0] + ".o",
                            "-o",
                            os.path.splitext(source)[0] + ".tmp",
                        ]
                    )
                    subprocess.call(
                        [
                            "objcopy",
                            "-O",
                            "binary",
                            "-j",
                            ".text",
                            os.path.splitext(source)[0] + ".tmp",
                            os.path.splitext(source)[0],
                        ]
                    )

        for source in listdir("wasmpy/x86/32"):
            if os.path.isdir(source):
                continue

            # assemble x86 32 bit specific instructions
            if os.path.splitext(source)[1].lower() == ".s":
                subprocess.call(
                    [
                        "as",
                        "--32",
                        source,
                        "-o",
                        os.path.splitext(source)[0] + ".o",
                    ]
                )

                if platform.system() == "Linux":
                    subprocess.call(
                        [
                            "ld",
                            "-melf_i386",
                            "--oformat",
                            "binary",
                            os.path.splitext(source)[0] + ".o",
                            "-o",
                            os.path.splitext(source)[0],
                        ]
                    )

                elif platform.system() == "Windows":
                    subprocess.call(
                        [
                            "ld",
                            "-mi386pe",
                            "-T",
                            "NUL",
                            "--image-base",
                            "0",
                            os.path.splitext(source)[0] + ".o",
                            "-o",
                            os.path.splitext(source)[0] + ".tmp",
                        ]
                    )

                    subprocess.call(
                        [
                            "objcopy",
                            "-O",
                            "binary",
                            "-j",
                            ".text",
                            os.path.splitext(source)[0] + ".tmp",
                            os.path.splitext(source)[0],
                        ]
                    )


class tidy(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        if os.path.exists("wasmpy/opcodes.cpp"):
            os.remove("wasmpy/opcodes.cpp")

        for file in (
            listdir("wasmpy/x86")
            + listdir("wasmpy/x86/64")
            + listdir("wasmpy/x86/32")
        ):
            if not os.path.isfile(file):
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
            consumes = data["consumes"]
            for group in data["opcodes"]:
                opcodes.update(
                    dict(
                        zip(
                            group["instructions"],
                            (
                                i + group["offset"]
                                for i in range(len(group["instructions"]))
                            ),
                        )
                    )
                )

        with open("wasmpy/opcodes.cpp", "w+") as out:
            bits = struct.calcsize("P")

            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.hpp"\n',
                    '#include "x86.hpp"\n\n',
                    "bytes decodeFunc(bytes buf, char plat, uint64_t globalTableAddr)\n{\n\t",
                    "std::vector<bytes> insts = {};\n\t",
                    "int localidx;\n\t",
                    "uint64_t hh, hl, lh, ll, bits;\n\t"
                    "for (size_t i = 0; i < buf.size(); i++)\n\t{\n\t\t",
                    "switch (buf.at(i))\n\t\t{\n\t\t",
                )
            )

            if bits == 8:
                extra = listdir("wasmpy/x86/64")

            elif bits == 4:
                extra = listdir("wasmpy/x86/32")

            for file in listdir("wasmpy/x86") + extra:
                if os.path.isdir(file):
                    continue

                inst = os.path.basename(file)

                if inst in opcodes:
                    with open(file, "rb") as fp:
                        data = ", ".join(str(i) for i in fp.read() if i != 0x90)

                    # alter binary data of instructions that take arguments
                    if inst in replacements:
                        for replacement in replacements[inst]:
                            data = data.replace(*replacement)

                    out.write(f"case {opcodes[inst]}:\n\t\t\t")
                    if inst in prefixed:
                        out.write("".join(prefixed[inst]))

                    out.write(f"insts.push_back({{{data}}});\n\t\t\t")

                    if inst in consumes:
                        out.write(f"i += {consumes[inst]};\n\t\t\t")

                    out.write("break;\n\n\t\t")

            out.writelines(
                (
                    "default:\n\t\t\t",
                    "break;\n\t\t",
                    "}\n\t}\n\t",
                    "return concat({}, insts);\n",
                    "}\n",
                )
            )


class build_ext(setuptools.command.build_ext.build_ext):
    def run(self):
        gen_opcodes.run(self)
        setuptools.command.build_ext.build_ext.run(self)


ext = []
if platform.machine() in ("x86", "i386", "i686", "AMD64", "x86_64"):
    ext = [
        setuptools.Extension(
            "wasmpy.x86",
            sources=[
                "wasmpy/x86.cpp",
                "wasmpy/opcodes.cpp",
            ],
            py_limited_api=True,
        )
    ]

with open("README.md", "r") as fp:
    description = fp.read()

setuptools.setup(
    name="wasmpy",
    version="0.1.4",
    author="Olivia Ryan",
    author_email="olivia.r.dev@gmail.com",
    description="Interactions between WebAssembly and Python",
    long_description=description,
    long_description_content_type="text/markdown",
    url="https://github.com/olivi-r/wasmpy",
    packages=["wasmpy"],
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
    python_requires=">=3.6",
    license="MIT",
)
