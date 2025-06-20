EMCC_FLAGS = -s STANDALONE_WASM --no-entry -Wl,--allow-undefined-file=wasm.syms
EXPORTED_FUNCS = -s EXPORTED_FUNCTIONS='["_step", "_init", "_get_adjMatrix", "_calculate_node_count", "_remove_node", "_get_active_node_count", "_get_node_colors", "_get_edge_strengths"]'

all: test.wasm physics_test.wasm

test.wasm: test.c viz.c
	emcc test.c viz.c -o test.wasm \
		$(EXPORTED_FUNCS) \
		$(EMCC_FLAGS)

physics_test.wasm: physics_test.c viz.c
	emcc physics_test.c viz.c -o physics_test.wasm \
		$(EXPORTED_FUNCS) \
		$(EMCC_FLAGS)