#include "viz.h"
#include <string.h>

int _adjMatrix[3][3] = {{0,1,1},{1,0,1},{0,1,1}};

void init(int* adjMatrix) {
    active_node_count = 3;

    strncpy(msg_buffer, "init", sizeof(msg_buffer));
    jsLog(msg_buffer);

    fill_adjMatrix((int *)_adjMatrix, 3);

    // Assign colors based on row and column position
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        node_colors[i] = (i % 3) + 1;
    }

    edge_strengths[0] = -10;
    for(int i=1; i<MATRIX_SIZE; i++){
        edge_strengths[i]=10;
    }
}

void step(int* adjMatrix) {
    active_node_count = calculate_node_count(adjMatrix);

    edge_strengths[active_node_count-1] *= -1;

    updateGraph();
}
