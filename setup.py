import setuptools.command.build_ext
import subprocess
import platform
import opcodes
import struct
import os


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
        with open("wasmpy/opcodes.cpp", "w+") as out:
            bits = struct.calcsize("P")

            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.hpp"\n',
                    '#include "x86.hpp"\n\n',
                    "bytes decodeFunc(bytes buf, char plat)\n{\n\t",
                    "std::vector<bytes> insts = {};\n\t",
                    "int localidx;\n\t",
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

                if inst in opcodes.opcodes:
                    with open(file, "rb") as fp:
                        data = ", ".join(str(i) for i in fp.read() if i != 0x90)

                    # alter binary data of instructions that take arguments
                    if inst in opcodes.replacements:
                        for replacement in opcodes.replacements[inst]:
                            data = data.replace(*replacement)

                    out.write(f"case {opcodes.opcodes[inst]}:\n\t\t\t")
                    if inst in opcodes.prefixed:
                        out.write("".join(opcodes.prefixed[inst]))

                    out.write(f"insts.push_back({{{data}}});\n\t\t\t")

                    if inst in opcodes.consumes:
                        out.write(f"i += {opcodes.consumes[inst]};\n\t\t\t")

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
    version="0.1.3",
    author="Olivia Ryan",
    author_email="olivia.r.dev@gmail.com",
    description="Interactions between WebAssembly and Python",
    long_description=description,
    long_description_content_type="text/markdown",
    url="https://github.com/olivi-r/wasmpy",
    packages=["wasmpy"],
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
