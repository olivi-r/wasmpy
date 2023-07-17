.PHONY: all
all: wasmpy/wasi/wasi_snapshot_preview1.so wasmpy/wasi/wasi_unstable.so

wasmpy/wasi/wasi_snapshot_preview1.so: build/wasi_snapshot_preview1.o build/wasi_common.o
	cc -fPIC -shared build/wasi/wasi_snapshot_preview1.o build/wasi/wasi_common.o -o wasmpy/wasi/wasi_snapshot_preview1.so

wasmpy/wasi/wasi_unstable.so: build/wasi_unstable.o build/wasi_common.o
	cc -fPIC -shared build/wasi/wasi_unstable.o build/wasi/wasi_common.o -o wasmpy/wasi/wasi_unstable.so

build/wasi_snapshot_preview1.o: build/wasi
	cc -fPIC -c wasmpy/wasi/wasi_snapshot_preview1.c -o build/wasi/wasi_snapshot_preview1.o

build/wasi_unstable.o: build/wasi
	cc -fPIC -c wasmpy/wasi/wasi_unstable.c -o build/wasi/wasi_unstable.o

build/wasi_common.o: build/wasi
	cc -fPIC -c wasmpy/wasi/wasi_common.c -o build/wasi/wasi_common.o

.PHONY: build/wasi
build/wasi:
	mkdir -p build/wasi

.PHONY: clean
clean:
	rm -rf build/wasi
