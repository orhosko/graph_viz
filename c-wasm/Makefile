test:
	clang test.c -Wall -Werror -fno-builtin --target=wasm32 --no-standard-libraries \
	-Wl,--no-entry -Wl,--export-all \
	-Wl,--allow-undefined-file=wasm.syms -o test.wasm