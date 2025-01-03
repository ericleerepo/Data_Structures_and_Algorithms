
/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA{1} 
* List.c
* List ADT
***/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "List.h"
#include "Matrix.h"

typedef struct EntryObj* Entry;

typedef struct EntryObj{
   double val;
   int col;
} EntryObj;

typedef struct MatrixObj* Matrix;

typedef struct MatrixObj{
   List* row;
   int num_rows;
   int nnz;
}MatrixObj;

Entry newEntry(double data, int c){
   Entry E = malloc(sizeof(EntryObj));
   assert( E!=NULL );
   E->val = data;
   E->col= c; //figure out what how to put pointers to correct node
   return(E);
}

void freeEntry(Entry* pE){
   if (pE != NULL && *pE != NULL) {
      free(*pE);
      *pE = NULL;
   }
}

Matrix newMatrix(int n){
   Matrix M = malloc(sizeof(MatrixObj));
   if(M==NULL){
      printf("Matrix Error: malloc failed\n");
      exit(EXIT_FAILURE);
   }
   M->row = calloc(n+1, sizeof(List));
   for (int i = 1; i <= n; i++){
      M->row[i] = newList();
   }
   M->num_rows = n;
   M->nnz = 0;
   return M;
}

int size(Matrix M){
   return M->num_rows;
}

int NNZ(Matrix M){
   return M->nnz;
}

int equals(Matrix A, Matrix B){
   if (size(A) != size(B)){
      return 0;
   }
   else{
      for(int i = 1; i<=size(A); i++){
         Entry EA;
         Entry EB;
         List LA = A->row[i];
         List LB = B->row[i];
         if(length(LA)!=length(LB)){
            return 0;
         }
         if(length(LA)==0){
            continue;
         }
         moveFront(LA);
         moveFront(LB);
         while(index(LA)>=0 && index(LB)>=0){
            EA = (Entry)get(LA);
            EB = (Entry)get(LB);
            if(EA->col != EB->col || EA->val != EB->val){
               return 0;
            }
            moveNext(LA);
            moveNext(LB);
            if(index(LA) != index(LB)){
               return 0;
            }
         }
      }
   }
   return 1;
}

void makeZero(Matrix M){
   for(int i = 1; i<=size(M); i++){
      clear(M->row[i]);
   }
   M->nnz = 0;
}

void freeMatrix(Matrix* pM){
   if (pM != NULL && *pM != NULL) {
      for (int i = 1; i <= size(*pM); i++){
         List L = (*pM)->row[i];
         if(length(L)>0){
            moveFront(L);
            while(index(L)>=0){
               Entry E = (Entry)get(L);
               freeEntry(&E);
               moveNext(L);
            }
         }
         freeList(&(*pM)->row[i]);
      }
      free((*pM)->row);
      free(*pM);
      *pM = NULL;
   }
}

void changeEntry(Matrix M, int i , int j, double x){
   if (1>j || j>size(M) || 1>i || i>size(M)){
      fprintf(stderr, "Calling changeEntry on invalid i or j");
      exit(EXIT_FAILURE);
   }
   List L= M->row[i];
   Entry E;
   if(length(L)==0){
      if(x!=0){
         append(L, newEntry(x,j));
         M->nnz++;
      }
      return;
   }
   moveFront(L);
   while(index(L)>=0){
      E = (Entry)get(L);
      if(E->col >= j){
         break;
      }
      moveNext(L);
   }
   if(x==0){
      if(E == NULL || index(L) == -1){
         return;
      }
      else if(E->col == j){
         delete(L);
         M->nnz--;
         return;
      }
   }
   if(E == NULL || index(L) == -1){ //if iter gets to end of L without finding correct spot just append, increment nnz
      append(L, newEntry(x,j));
      M->nnz++;
   }
   else if(E->col > j){ // if iter passes correct spot  for entry then insert before, increment nnz
      insertBefore(L, newEntry(x,j));
      M->nnz++;
   }
   else if(E->col == j){
      freeEntry(&E);
      set(L, newEntry(x,j));
   } //then just change value at entry, do not increment nnz
}

Matrix copy(Matrix A){
   Matrix C = newMatrix(size(A));
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      if(length(LA)==0){
         continue;
      }
      moveFront(LA);
      while(index(LA)>=0){
         Entry E = (Entry)get(LA);
         changeEntry(C, i, E->col, E->val);
         moveNext(LA);
      }
   }
   return C;
}

Matrix transpose(Matrix A){
   Matrix t = newMatrix(size(A));
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      if(length(LA)==0){
         continue;
      }
      moveFront(LA);
      while(index(LA)>=0){
         Entry E = (Entry)get(LA);
         changeEntry(t, E->col, i, E->val);
         moveNext(LA);
      }
   }
   return t;
}

Matrix scalarMult(double x, Matrix A){
   Matrix s = newMatrix(size(A));
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      if(length(LA)==0){
         continue;
      }
      moveFront(LA);
      while(index(LA)>=0){
         Entry E = (Entry)get(LA);
         double mul = x*E->val;
         changeEntry(s, i, E->col, mul);
         moveNext(LA);
      }
   }
   return s;
}

Matrix sum(Matrix A, Matrix B){
   if(size(A)!=size(B)){
      fprintf(stderr, "Matrix Error: calling sum() on differnt size matricies\n");
      exit(EXIT_FAILURE);
   }
   Matrix c = newMatrix(size(A));
   Matrix CB = copy(B);
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      List LB = CB->row[i];
      if(length(LA)==0 && length(LB)==0){
         continue;
      }
      else if(length(LA)==0){
         moveFront(LB);
         while(index(LB)>=0){
            Entry EB = (Entry)get(LB);
            changeEntry(c, i, EB->col, EB->val);
            moveNext(LB);
         }
         continue;
      }
      else if(length(LB)==0){
         moveFront(LA);
         while(index(LA)>=0){
            Entry EA = (Entry)get(LA);
            changeEntry(c, i, EA->col, EA->val);
            moveNext(LA);
         }
         continue;
      }
      moveFront(LA);
      moveFront(LB);

      while(index(LA)>=0 || index(LB)>=0){
         Entry EA = (index(LA)>=0) ? (Entry)get(LA) : NULL;
         Entry EB = (index(LB) >= 0) ? (Entry)get(LB) : NULL;
         if(EA!=NULL && EB!=NULL){
            if(EA->col == EB->col){
               double sum = EA->val+EB->val;
               changeEntry(c , i, EA->col, sum);
               moveNext(LA);
               moveNext(LB);
            }
            else if (EA->col < EB->col){
               changeEntry(c, i, EA->col, EA->val);
               moveNext(LA);
            }else if(EA->col > EB->col){
               changeEntry(c, i, EB->col, EB->val);
               moveNext(LB);
            }
         }
         else if(EA == NULL){
            changeEntry(c, i, EB->col, EB->val);
            moveNext(LB);
         }
         else if(EB == NULL){
            changeEntry(c, i, EA->col, EA->val);
            moveNext(LA);
         }
      }
   }
   freeMatrix(&CB);
   return c;
}

Matrix diff(Matrix A, Matrix B){
   if(size(A)!=size(B)){
      fprintf(stderr, "Matrix Error: calling diff() on differnt size matricies\n");
      exit(EXIT_FAILURE);
   }
   Matrix c = newMatrix(size(A));
   Matrix CB = copy(B);
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      List LB = CB->row[i];
      if(length(LA)==0 && length(LB)==0){
         continue;
      }
      else if(length(LA)==0){
         moveFront(LB);
         while(index(LB)>=0){
            Entry EB = (Entry)get(LB);
            changeEntry(c, i, EB->col, -EB->val);
            moveNext(LB);
         }
         continue;
      }
      else if(length(LB)==0){
         moveFront(LA);
         while(index(LA)>=0){
            Entry EA = (Entry)get(LA);
            changeEntry(c, i, EA->col, EA->val);
            moveNext(LA);
         }
         continue;
      }
      moveFront(LA);
      moveFront(LB);

      while(index(LA)>=0 || index(LB)>=0){
         Entry EA = (index(LA)>=0) ? (Entry)get(LA) : NULL;
         Entry EB = (index(LB) >= 0) ? (Entry)get(LB) : NULL;
         if(EA!=NULL && EB!=NULL){
            if(EA->col == EB->col){
               double diff = EA->val-EB->val;
               changeEntry(c , i, EA->col, diff);
               moveNext(LA);
               moveNext(LB);
            }
            else if (EA->col < EB->col){
               changeEntry(c, i, EA->col, EA->val);
               moveNext(LA);
            } else if(EA->col > EB->col){
               changeEntry(c, i, EB->col, -EB->val);
               moveNext(LB);
            }
         }
         else if(EA == NULL){
            changeEntry(c, i, EB->col, -EB->val);
            moveNext(LB);
         }
         else{
            changeEntry(c, i, EA->col, EA->val);
            moveNext(LA);
         }
      }
   }
   freeMatrix(&CB);
   return c;
}

Matrix product(Matrix A, Matrix B){
   if (size(A) != size(B)) {
      fprintf(stderr, "Matrix Error: calling product() on differnet size matricies\n");
      exit(EXIT_FAILURE);
   }
   Matrix C = newMatrix(size(A));
   Matrix BT = transpose(B);
   for(int i = 1; i<=size(A); i++){
      List LA = A->row[i];
      if(length(LA)==0){
         continue;
      }
      for(int j = 1; j<=size(A); j++){
         List LBT = BT->row[j];
         if(length(LBT) == 0){
            continue;
         }
         moveFront(LA);
         moveFront(LBT);
         double dot = 0.0;
         while(index(LA)>=0 && index(LBT)>=0){
            Entry EA = (index(LA)>=0) ? (Entry)get(LA) : NULL;
            Entry EB = (index(LBT) >= 0) ? (Entry)get(LBT) : NULL;
            if(EA!=NULL && EB!=NULL){
               if(EA->col == EB->col){
                  dot += EA->val*EB->val;
                  moveNext(LA);
                  moveNext(LBT);
               }
               else if (EA->col < EB->col){
                  moveNext(LA);
               } else{
                  moveNext(LBT);
               }
            }
         }
         changeEntry(C, i, j, dot);
      }
   }
   freeMatrix(&BT);
   return C;
}

void printMatrix(FILE* out, Matrix M) {
   if (M == NULL) {
      fprintf(stderr, "Matrix Error: calling printMatrix() on a NULL Matrix reference\n");
      exit(EXIT_FAILURE);
   }

    // Iterate over each row of the matrix
   for (int i = 1; i <= size(M); i++) {
      List rowList = M->row[i];
      if (length(rowList) == 0) {
         continue;  // Skip zero rows
      }

        // Print the row number
      fprintf(out, "%d: ", i);

        // Traverse the entries in the current row list
      moveFront(rowList);
      while (index(rowList) >= 0) {
         Entry E = (Entry)get(rowList);
            // Print the entry in the format "(col, val)" rounded to 1 decimal place
         fprintf(out, "(%d, %.1f)", E->col, E->val);
         moveNext(rowList);

            // If not the last element, add a space between entries
         if (index(rowList) >= 0) {
            fprintf(out, " ");
         }
      }

        // End the line for the current row
      fprintf(out, "\n");
   }
}
