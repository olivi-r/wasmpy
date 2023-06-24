import setuptools.command.build_ext
import subprocess
import opcodes
import os


class Assemble(setuptools.command.build_ext.build_ext):
    def run(self):
        for source in os.listdir("wasmpy/x86/instructions"):
            if os.path.isdir(os.path.join("wasmpy/x86/instructions", source)):
                continue

            # assemble instructions
            if os.path.splitext(source)[1].lower() == ".asm":
                subprocess.call(
                    [
                        "nasm",
                        os.path.join("wasmpy/x86/instructions", source),
                        "-fbin",
                    ]
                )

        # generate opcodes.cpp
        with open("wasmpy/x86/opcodes.cpp", "w+") as out:
            out.writelines(
                (
                    "// auto-generated\n\n",
                    '#include "opcodes.h"\n',
                    '#include "helpers.h"\n\n',
                    "bytes decodeFunc(bytes buf, char plat)\n{\n\t",
                    "int localidx;\n\t",
                    "std::vector<bytes> insts = {};\n\t",
                    "for (size_t i = 0; i < buf.size(); i++)\n\t{\n\t\t",
                    "switch (buf.at(i))\n\t\t{\n\t\t",
                )
            )

            for file in os.listdir("wasmpy/x86/instructions"):
                if os.path.isdir(os.path.join("wasmpy/x86/instructions", file)):
                    continue

                if os.path.splitext(file)[1].lower() != ".asm":
                    with open(
                        os.path.join("wasmpy/x86/instructions", file), "rb"
                    ) as fp:
                        data = ", ".join(str(i) for i in fp.read())

                    out.writelines(
                        (
                            f"case {opcodes.opcodes[file]}:\n\t\t\t",
                            f"insts.push_back({{{data}}});\n\t\t\t",
                            "break;\n\n\t\t",
                        )
                    )

            out.writelines(
                (
                    "default:\n\t\t\t",
                    "break;\n\t\t",
                    "}\n\t}\n\t",
                    "return concat({}, insts);\n",
                    "}\n",
                )
            )

        setuptools.command.build_ext.build_ext.run(self)


with open("README.md", "r") as fp:
    description = fp.read()

setuptools.setup(
    name="wasmpy",
    version="0.1.0",
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
    cmdclass={"build_ext": Assemble},
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
