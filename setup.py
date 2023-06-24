import setuptools.command.build_ext
import subprocess
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
        for source in listdir("wasmpy/x86/instructions"):
            if os.path.isdir(source):
                continue

            # assemble instructions
            if os.path.splitext(source)[1].lower() == ".asm":
                subprocess.call(["nasm", source, "-fbin"])

        for source in listdir("wasmpy/x86/instructions/x64"):
            if os.path.isdir(source):
                continue

            # assemble x64 specific instructions
            if os.path.splitext(source)[1].lower() == ".asm":
                subprocess.call(["nasm", source, "-fbin"])

        for source in listdir("wasmpy/x86/instructions/x86"):
            if os.path.isdir(source):
                continue

            # assemble x86 specific instructions
            if os.path.splitext(source)[1].lower() == ".asm":
                subprocess.call(["nasm", source, "-fbin"])


class gen_opcodes(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        # generate opcodes.cpp
        with open("wasmpy/x86/opcodes.cpp", "w+") as out:
            bits = struct.calcsize("P")

            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.h"\n',
                    '#include "helpers.h"\n\n',
                    "bytes decodeFunc(bytes buf, char plat)\n{\n\t",
                    "std::vector<bytes> insts = {};\n\t",
                    "for (size_t i = 0; i < buf.size(); i++)\n\t{\n\t\t",
                    "int localidx = buf.at(i + 4) << 24 | buf.at(i + 3) << 16 | buf.at(i + 2) << 8 | buf.at(i + 1);\n\t\t",
                    "localidx *= 10;\n\t\t",
                    "switch (buf.at(i))\n\t\t{\n\t\t",
                )
            )

            if bits == 8:
                extra = listdir("wasmpy/x86/instructions/x64")

            elif bits == 4:
                extra = listdir("wasmpy/x86/instructions/x86")

            for file in listdir("wasmpy/x86/instructions") + extra:
                if os.path.isdir(file):
                    continue

                inst = os.path.basename(file)

                if os.path.splitext(file)[1].lower() != ".asm":
                    with open(file, "rb") as fp:
                        data = ", ".join(str(i) for i in fp.read())

                    # alter binary data of instructions that take arguments
                    if inst in opcodes.replacements:
                        for replacement in opcodes.replacements[inst]:
                            data = data.replace(*replacement)

                    out.writelines(
                        (
                            f"case {opcodes.opcodes[inst]}:\n\t\t\t",
                            f"insts.push_back({{{data}}});\n\t\t\t",
                        )
                    )

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


with open("README.md", "r") as fp:
    description = fp.read()

setuptools.setup(
    name="wasmpy",
    version="0.1.1",
    author="Olivia Ryan",
    author_email="olivia.r.dev@gmail.com",
    description="Interactions between WebAssembly and Python",
    long_description=description,
    long_description_content_type="text/markdown",
    url="https://github.com/olivi-r/wasmpy",
    packages=["wasmpy"],
    ext_modules=[
        setuptools.Extension(
            "wasmpy.win_x86",
            sources=[
                "wasmpy/win_x86.cpp",
                "wasmpy/x86/opcodes.cpp",
                "wasmpy/x86/helpers.cpp",
            ],
            py_limited_api=True,
        )
    ],
    options={"bdist_wheel": {"py_limited_api": "cp36"}},
    cmdclass={
        "assemble": assemble,
        "build_ext": build_ext,
        "genopcodes": gen_opcodes,
    },
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
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
