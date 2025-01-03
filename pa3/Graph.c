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
   int* discover;
   int* finish;
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
   G->discover = calloc(n + 1, sizeof(int));
   G->finish = calloc(n + 1, sizeof(int));
   for (int i = 1; i <= n; i++) {
      G->discover[i] = UNDEF;
   }
   for (int i = 1; i <= n; i++) {
      G->finish[i] = UNDEF;
   }
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
       free((*pG)->discover);
       free((*pG)->finish);
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

int getParent(Graph G, int u){
   if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "Graph Error: calling getParent() with invalid vertex\n");
    exit(EXIT_FAILURE);
   }
   return G->parent[u];
}

int getDiscover(Graph G, int u){
   if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "Graph Error: calling getDist() with invalid vertex\n");
    exit(EXIT_FAILURE);
   }
   return G->discover[u];
}

int getFinish(Graph G, int u){
   if (u < 1 || u > getOrder(G)) {
    fprintf(stderr, "Graph Error: calling getFinish() with invalid vertex\n");
    exit(EXIT_FAILURE);
   }
   return G->finish[u];
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
      if(v == get(adj)){
         G->size--;
      }
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

void Visit(Graph G, int x, int* time,List s){
   G->discover[x] = ++(*time);
   G->color[x] = 'G';
   for(moveFront(G->neighbors[x]); index(G->neighbors[x])>=0; moveNext(G->neighbors[x])){
      if (G->color[get(G->neighbors[x])] == 'W'){
         G->parent[get(G->neighbors[x])] = x;
         Visit(G, get(G->neighbors[x]) , time, s);
      }
   }
   G->color[x] = 'B';
   G->finish[x] = ++(*time);
   prepend(s, x);
}

void DFS(Graph G, List s){
   if(length(s)!=getOrder(G)){
      fprintf(stderr, "DFS Error: List is not equal length\n");
      exit(EXIT_FAILURE);
   }
   List order = copyList(s);
   clear(s);
   for (int i = 1; i <=getOrder(G); i++){
      G->color[i] = 'W';
      G->parent[i] = NIL;
      G->discover[i] = UNDEF;
      G->finish[i] = UNDEF;
   }
   int time = 0;
   for (moveFront(order); index(order)>=0; moveNext(order)){
      if(G->color[get(order)]=='W'){
         Visit(G, get(order), &time, s);
      }
   }
   freeList(&order);
}

Graph transpose(Graph G){
   Graph t = newGraph(getOrder(G));
   for(int i = 1; i<=getOrder(t); i++){
      for(moveFront(G->neighbors[i]); index(G->neighbors[i])>=0; moveNext(G->neighbors[i])){
         append(t->neighbors[get(G->neighbors[i])], i);
      }
   }
   return t;
}

Graph copyGraph(Graph G){
   Graph copy = newGraph(getOrder(G));
   for(int i = 1; i<=getOrder(copy); i++){
      for(moveFront(G->neighbors[i]); index(G->neighbors[i])>=0; moveNext(G->neighbors[i])){
         append(copy->neighbors[i],get(G->neighbors[i]));
      }
   }
   return copy;
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

