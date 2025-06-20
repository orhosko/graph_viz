#pragma once

#include <stdio.h>
#include <string.h>

// Functions from JS
extern void jsLog(char* msg);
extern void updateGraph();
extern int rand();

#define MATRIX_SIZE 64

extern int adjMatrix[MATRIX_SIZE * MATRIX_SIZE];
extern int node_colors[MATRIX_SIZE];
extern int in_degrees[MATRIX_SIZE];
extern int out_degrees[MATRIX_SIZE];
extern int edge_strengths[MATRIX_SIZE];

extern char msg_buffer[128];

extern int active_node_count;

int* get_adjMatrix();
int* get_node_colors();
int* get_in_degrees();
int* get_out_degrees();
int* get_edge_strengths();

void fill_adjMatrix(int* smaller, int n);

int calculate_node_count(int* adjMatrix);
int get_active_node_count();
void remove_node(int* adjMatrix, int active_node_count, int node_index);
void merge_nodes(int* adjMatrix, int active_node_count, int node1, int node2);

int node_out_degree(int *adjMatrix, int node_index);
void calculate_out_degrees(int *adjMatrix);

int node_in_degree(int *adjMatrix, int node_index);
void calculate_in_degrees(int *adjMatrix);
