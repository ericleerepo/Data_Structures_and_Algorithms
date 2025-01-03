/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA2
* Graph.c
* Graph ADT implementation and BFS algorithm
***/ 
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "Graph.h"
#include "List.h"

typedef struct GraphObj* Graph;

typedef struct GraphObj{
   List* neighbors;
   char* color;
   int* parent;
   int* distance;
   int order;
   int size;
   int source;  
}GraphObj;

Graph newGraph(int n){
   Graph G;
   G = malloc(sizeof(GraphObj));
   G->order = n;
   G->size = 0;
   G->source = NIL;
   G->neighbors = calloc(n+1, sizeof(List));
   for (int i = 1; i <= n; i++) {
      G->neighbors[i] = newList();
   }
   G->color = calloc(n + 1, sizeof(char));
   G->parent = calloc(n + 1, sizeof(int));
   G->distance = calloc(n + 1, sizeof(int));
   return G;
}

void freeGraph(Graph* pG){
   if (pG != NULL && *pG != NULL) {
       for (int i = 1; i <= getOrder(*pG); i++) {
           freeList(&(*pG)->neighbors[i]);
       }
       free((*pG)->neighbors);
       free((*pG)->color);
       free((*pG)->parent);
       free((*pG)->distance);
       free(*pG);
       *pG = NULL;
   }
}

int getOrder(Graph G){
   return G->order;
}

int getSize(Graph G){
   return G->size;
}

int getSource(Graph G){
   return G->source;
}

int getParent(Graph G, int u){
   if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "Graph Error: calling getParent() with invalid vertex\n");
    exit(EXIT_FAILURE);
   }
   if (G->source == NIL) {
      return NIL; 
   }
   return G->parent[u];
}

int getDist(Graph G, int u){
   if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "Graph Error: calling getDist() with invalid vertex\n");
    exit(EXIT_FAILURE);
   }
   if (G->source == NIL) {
      return INF; 
   }
   return G->distance[u];
}

void getPath(List L, Graph G, int u){
   if (getSource(G) == NIL){
      fprintf(stderr, "Graph Error: calling getPath() with no source\n");
      exit(EXIT_FAILURE);
   }
   if (u < 1 || u > getOrder(G)) {
      fprintf(stderr, "Graph Error: calling getPath() with invalid vertex\n");
      exit(EXIT_FAILURE);
   }
   if (u == getSource(G)) {
      append(L, u);
   } else if (getParent(G, u) == NIL) {
      append(L, NIL);
   } else {
      getPath(L, G, getParent(G, u));
      append(L, u);
   }
}

void makeNull(Graph G){
   for (int i = 1; i <= getOrder(G); i++) {
      clear(G->neighbors[i]);
   }
   G->size = 0;
}

void addEdge(Graph G, int u, int v){
   if (u > getOrder(G) || u < 1 || v > getOrder(G) || v < 1){
      fprintf(stderr, "Graph Error: calling addEdge() invalid int u:%d v:%d\n", u, v);
      exit(EXIT_FAILURE);
   }
   List adj = G->neighbors[u];
   moveFront(adj);
   while (index(adj) >= 0) {
      if (v < get(adj)) {
         insertBefore(adj, v);
         break;
      }
      moveNext(adj);
   }
   if (index(adj) == -1) {
      append(adj, v);
   }
   List adj2 = G->neighbors[v];
   moveFront(adj2);
   while (index(adj2) >= 0) {
      if (u < get(adj2)) {
         insertBefore(adj2, u);
         break;
      }
      moveNext(adj2);
   }
   if (index(adj2) == -1) {
      append(adj2, u);
   }
   G->size++;
}

void addArc(Graph G, int u , int v){
   if (u > getOrder(G) || u < 1 || v > getOrder(G) || v < 1){
      fprintf(stderr, "Graph Error: calling addArc() invalid int\n");
      exit(EXIT_FAILURE);
   }
   List adj = G->neighbors[u];
   moveFront(adj);
   while (index(adj) >= 0) {
      if (v < get(adj)) {
         insertBefore(adj, v);
         break;
      }
      moveNext(adj);
   }
   if (index(adj) == -1) {
      append(adj, v);
   }
   G->size++;
}

void BFS(Graph G, int s){
   if (s > getOrder(G) || s < 1){
      fprintf(stderr, "Graph Error: calling BFS() invalid int\n");
      exit(EXIT_FAILURE);
   }
   G->source = s;
   for (int i = 1; i <= getOrder(G); i++) {
      G->color[i] = 'w';
      G->parent[i] = NIL;
      G->distance[i] = INF;
   }
   G->color[s] = 'g';
   G->distance[s] = 0;
   G->parent[s] = NIL;
   List Q = newList();
   assert(Q != NULL);
   append(Q, s);
   while(length(Q)>0){
      int x = front(Q);
      deleteFront(Q);
      List adj = G->neighbors[x];
      assert(adj != NULL);
      moveFront(adj);
      while (index(adj) >= 0) {
         int y = get(adj);
         if (G->color[y] == 'w') {
            G->color[y] = 'g';
            G->distance[y] = G->distance[x] + 1;
            G->parent[y] = x;
            append(Q, y);
         }  
         moveNext(adj);
      }
      G->color[x] = 'b';
   }
   freeList(&Q);
}

void printGraph(FILE* out, Graph G){
   for(int i = 1; i <= G->order; i++){
      fprintf(out, "%d: ", i);
      List adj = G->neighbors[i];
      moveFront(adj);
      while(index(adj)>=0){
         fprintf(out, "%d ", get(adj));
         moveNext(adj);
      }
      fprintf(out, "\n");
   }
}

