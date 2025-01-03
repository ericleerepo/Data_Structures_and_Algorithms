/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA2
* GraphTest.c 
* Main function for shortest path between verticies on a given graph
***/ 
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "List.h"
#include "Graph.h"

int main(int argc, char * argv[]){
   FILE *in, *out;


   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   in = fopen(argv[1], "r");
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   out = fopen(argv[2], "w");
   if(out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      fclose(in);
      exit(1);
   }

   int n;
   fscanf(in, "%d", &n);
   Graph G = newGraph(n);

   int u, v;
   while (fscanf(in, "%d %d", &u, &v) == 2) {
      if (u == 0 && v == 0) {
         break; 
      }
      addEdge(G, u, v);  
   }
   printGraph(out, G);

   fprintf(out, "\n");

   int source, end;
   while (fscanf(in, "%d %d", &source, &end) == 2) {
      if (source == 0 && end == 0) {
         break; 
      }
      BFS(G, source);
      List path = newList();
      getPath(path, G, end);
      if(getDist(G, end) == INF){
         fprintf(out, "The distance from %d to %d is infinity\n", source, end);
         fprintf(out, "No %d-%d path exists", source, end);
         fprintf(out, "\n");
      }else{
         fprintf(out, "The distance from %d to %d is %d\n", source, end, getDist(G, end));
         fprintf(out, "A shortest %d-%d path is: ", source, end);
         moveFront(path);
         while (index(path) >= 0) {
            fprintf(out, "%d ", get(path));
            moveNext(path);
         }
         fprintf(out, "\n");
      }
      freeList(&path);
      fprintf(out, "\n");
   }
   freeGraph(&G);
   fclose(in);
   fclose(out);
}