#include <array>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>

std::array<std::array<int, 16>, 16> adjMatrix{};

void create_row_deps() {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i % 4 != j) {
                adjMatrix[i][(i / 4) * 4 + j] += 1;
            }
        }
    }
}

void create_col_deps() {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i / 4 != j) {
                adjMatrix[i][(i % 4) + j * 4] += 1;
            }
        }
    }
}

void create_block_deps() {
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

void normalize_adj_matrix() {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            adjMatrix[i][j] = std::min(adjMatrix[i][j], 1);
        }
    }
}

// Function to get the adjacency matrix as a flat vector
std::vector<int> get_adjacency_matrix() {
    std::vector<int> flat_matrix;
    flat_matrix.reserve(256); // 16x16
    
    for (const auto& row : adjMatrix) {
        for (int val : row) {
            flat_matrix.push_back(val);
        }
    }
    
    return flat_matrix;
}

// Main function to run the algorithm and return the result
std::vector<int> run() {
    create_row_deps();
    create_col_deps();
    create_block_deps();
    normalize_adj_matrix();

    // Print the matrix to console for debugging
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return get_adjacency_matrix();
}

// Binding code
EMSCRIPTEN_BINDINGS(module) {
    emscripten::register_vector<int>("IntVector");
    emscripten::function("run", &run);
}