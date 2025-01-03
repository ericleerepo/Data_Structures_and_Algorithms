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
#include "Matrix.h"

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

   int n, a, b;
   fscanf(in, "%d %d %d", &n, &a, &b);
   Matrix A = newMatrix(n);
   Matrix B = newMatrix(n);

   fscanf(in, "\n");

   int row, col;
   double value;

   for (int i = 0; i < a; i++) {
      fscanf(in, "%d %d %lf", &row, &col, &value);
      changeEntry(A, row, col, value);
   }

   fscanf(in, "\n");

   for (int i = 0; i < b; i++) {
      fscanf(in, "%d %d %lf", &row, &col, &value);
      changeEntry(B, row, col, value);
   }

   fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
   printMatrix(out, A);

   fprintf(out, "\n");

   fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
   printMatrix(out, B);

   fprintf(out, "\n");

   fprintf(out, "(1.5)*A =\n");
   Matrix C = scalarMult(1.5, A);
   printMatrix(out, C);

   fprintf(out, "\n");

   fprintf(out, "A+B =\n");
   Matrix D = sum(A,B);
   printMatrix(out, D);

   fprintf(out, "\n");

   fprintf(out, "A+A =\n");
   Matrix E = sum(A,A);
   printMatrix(out, E);

   fprintf(out, "\n");

   fprintf(out, "B-A =\n");
   Matrix F = diff(B,A);
   printMatrix(out, F);

   fprintf(out, "\n");

   fprintf(out, "A-A =\n");
   Matrix G = diff(A, A);
   printMatrix(out, G);

   fprintf(out, "\n");

   fprintf(out, "Transpose(A) =\n");
   Matrix H = transpose(A);
   printMatrix(out, H);

   fprintf(out, "\n");

   fprintf(out, "A*B =\n");
   Matrix I = product(A,B);
   printMatrix(out, I);

   fprintf(out, "\n");

   fprintf(out, "B*B =\n");
   Matrix J = product(B,B);
   printMatrix(out, J);

   freeMatrix(&A);
   freeMatrix(&B);
   freeMatrix(&C);
   freeMatrix(&D);
   freeMatrix(&E);
   freeMatrix(&F);
   freeMatrix(&G);
   freeMatrix(&H);
   freeMatrix(&I);
   freeMatrix(&J);
}