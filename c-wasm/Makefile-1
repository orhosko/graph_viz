.PHONY: serve

add.wasm: add.cpp
	clang --target=wasm32 -O3 -flto -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--lto-O3 -o add.wasm add.cpp

serve:
	python -m http.server 6969

LINK_FLAGS "-s WASM=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=['run'] -lembind --bind"
emcc add.cpp -o add_module.wasm -s EXPORTED_FUNCTIONS='["_add","_run"]' -s STANDALONE_WASM --no-entry