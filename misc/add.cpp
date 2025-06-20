#include <emscripten.h>
#include <stdlib.h>

EMSCRIPTEN_KEEPALIVE
extern "C" {
int* add(int a, int b) { return (int*)(a * a) + b; }

int* run() {
    int* a = (int*)malloc(sizeof(int));
    *a = 1;
    return a;
}
}

/*
struct Square {
    // int x;
    // int y;
    int index;
    int value;
};

// TODO: make constexpr

std::array<std::array<Square, 4>, 4> board;

std::array<std::array<int, 16>, 16> adjMatrix{};

void create_row_deps(std::array<std::array<int, 16>, 16> &adjMartix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i % 4 != j) {
                adjMatrix[i][(i / 4) * 4 + j] += 1;
            }
        }
    }
}

void create_col_deps(std::array<std::array<int, 16>, 16> adjMartix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i / 4 != j) {
                adjMatrix[i][(i % 4) + j * 4] += 1;
            }
        }
    }
}

void create_block_deps(std::array<std::array<int, 16>, 16> adjMartix) {
    for (int i = 0; i < 16; ++i) {
        int x = i % 4;
        int y = i / 4;
        int block_index = (y / 2) * 2 + (x / 2);

        int block_min = (block_index % 2) * 2 + (block_index / 2) * 8;

        adjMatrix[i][block_min] += 1;
        adjMatrix[i][block_min + 1] += 1;
        adjMatrix[i][block_min + 4] += 1;
        adjMatrix[i][block_min + 5] += 1;

        adjMatrix[i][i] -= 1;
    }
}

void normalize_adj_matrix(std::array<std::array<int, 16>, 16> &adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
          adjMatrix[i][j] = std::min(adjMatrix[i][j], 1);
        }
    }
}

int run() {
    create_row_deps(adjMatrix);
    create_col_deps(adjMatrix);
    create_block_deps(adjMatrix);

    normalize_adj_matrix(adjMatrix);

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

*/