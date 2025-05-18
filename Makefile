.PHONY: serve

add.wasm: add.cpp
	clang --target=wasm32 -O3 -flto -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--lto-O3 -o add.wasm add.cpp

serve:
	python -m http.server 6969
