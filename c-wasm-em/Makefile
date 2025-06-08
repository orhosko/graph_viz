test:
	clang test.c -Wall -Werror -fno-builtin --target=wasm32 --no-standard-libraries \
	-Wl,--no-entry -Wl,--export-all \
	-Wl,--allow-undefined-file=wasm.syms -o test.wasm

test2:
	emcc test.c -o test.wasm -s EXPORTED_FUNCTIONS='["_step","_run","_init", "_get_adjMatrix", "_get_node_colors", "_get_active_node_count"]' -s STANDALONE_WASM --no-entry -Wl,--allow-undefined-file=wasm.syms

em-prep:
	source ../../emsdk/emsdk_env.sh