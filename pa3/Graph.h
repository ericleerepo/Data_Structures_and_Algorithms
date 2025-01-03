/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA2
* Graph.h
* Header File for Graph.c
***/ 
#include<stdbool.h>
#include"List.h"
#define UNDEF -1
#define NIL 0

typedef struct GraphObj* Graph;

Graph newGraph(int n);

void freeGraph(Graph* pG);

int getOrder(Graph G);

int getSize(Graph G);

int getSource(Graph G);

int getParent(Graph G, int u);

int getDiscover(Graph G, int u);

int getFinish(Graph G, int u);

void makeNull(Graph G);

void addEdge(Graph G, int u, int v);

void addArc(Graph G, int u , int v);

void DFS(Graph G, List s);

Graph transpose(Graph G);

Graph copyGraph(Graph G);

void printGraph(FILE* out, Graph G);

