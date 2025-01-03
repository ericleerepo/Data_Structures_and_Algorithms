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
      addArc(G, u, v);  
   }
   fprintf(out, "Adjacency list representation of G:\n");
   printGraph(out, G);

  List s = newList();

   for(int i = 1; i<=n; i++){
      append(s, i);
   }

   DFS(G, s);
   Graph Gt = transpose(G);
   DFS(Gt, s);

/*   List s = newList();
   for (moveFront(x); index(x)>=0; moveNext(x)){
      prepend(s, get(x));
   }*/

   int count = 0;
   for (moveFront(s); index(s)>=0; moveNext(s)){
      if(getParent(Gt, get(s)) == NIL){
         count++;
      }
   }
   fprintf(out, "\n");
   fprintf(out,"G contains %d strongly connected components:\n", count);
   /*for (moveFront(s); index(s)>=0; moveNext(s)){
      fprintf(out, "%d ", getParent(G,get(s)));
   }
   fprintf(out, "\n");
   printList(out, s);
   fprintf(out, "\n");
*/
   List* components=calloc(count+1, sizeof(List));
   for(int i = 1; i<=count; i++){   
      components[i] = newList();
   }
   moveBack(s);
   int i = 1;
   while(index(s)>=0){
      prepend(components[i], get(s));
      if (getParent(Gt,get(s))==NIL){
         i++;
      }
      movePrev(s);
   }
   for(int i = 1; i<=count; i++){   
      fprintf(out, "Component %d: ", i);
      printList(out, components[i]);
      fprintf(out,"\n");
   }

   freeGraph(&G);
   freeGraph(&Gt);
   freeList(&s);

   fclose(in);
   fclose(out);
}
