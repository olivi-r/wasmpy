cmake_minimum_required(VERSION 3.10)
project(wasmpy)

file(GLOB_RECURSE SOURCES wasmpy/arch/*.asm)
string(REPLACE ".asm" "" BINARIES "${SOURCES}")

add_custom_target(assemblies DEPENDS ${BINARIES})
foreach(source ${SOURCES})
    string(REPLACE ".asm" "" binary "${source}")
    add_custom_command(OUTPUT ${binary} COMMAND nasm -f bin ${source})
endforeach()
