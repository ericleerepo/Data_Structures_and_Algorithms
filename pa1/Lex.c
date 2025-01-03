/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA{1} 
* List.c 
* Lex implementation
***/ 
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "List.h"

#define MAX_LEN 300

int main(int argc, char * argv[]){
   int line_count = 0;
   FILE *in, *out;
   char line[MAX_LEN];
   char **linesArray;


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
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      fclose(in);
      exit(1);
   }


   while (fgets(line, MAX_LEN, in) != NULL) {
      line_count++;
   }


   linesArray = (char **)calloc(line_count, sizeof(char *));
   if (linesArray == NULL) {
      printf("Error: Unable to allocate memory for lines array\n");
      exit(1);
   }

   rewind(in);  


   int i = 0;
   while (fgets(line, MAX_LEN, in) != NULL) {
      size_t len = strlen(line);
      if (len > 0 && line[len - 1] == '\n') {
         line[len - 1] = '\0';  
      }
      linesArray[i] = (char *)malloc((strlen(line) + 1) * sizeof(char));
      if (linesArray[i] == NULL) {
         printf("Error: Unable to allocate memory for line %d\n", i);
         exit(1);
      }
      strcpy(linesArray[i], line); 
      i++;
   }


   List L = newList();
   append(L, 0);


   for(i = 1; i < line_count; i++){  
      moveFront(L);
      

      while(index(L) >= 0){
         int idx = get(L);
         if(strcmp(linesArray[i], linesArray[idx]) < 0){
            insertBefore(L, i);
            break;
         }
         moveNext(L);
      }
      
     
      if (index(L) == -1){
         append(L, i);
      }

      
   }


   //printf("About to call printList...\n");
   moveFront(L);  
   while (index(L) >= 0) {
      int idx = get(L);
      fprintf(out, "%s\n", linesArray[idx]);  
      moveNext(L);
   }
   //printf("Finished calling printList.\n");


   freeList(&L);
   //printf("reached?\n");
   for (i = 0; i < line_count; i++){
      free(linesArray[i]); 
   }
   free(linesArray);  
   
   fclose(in);
   fclose(out);
   
}