//-----------------------------------------------------------------------------
//  MatrixClient.c 
//  A test client for the Matrix ADT
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"Matrix.h"
   
int main(){
   int n=100000;
   Matrix A = newMatrix(n);
   Matrix B = newMatrix(n);
   //Matrix C, D, E, F, G, H;
   Matrix H, F, C, D, E, G;

   changeEntry(A, 1,1,1); changeEntry(B, 1,1,1);
   changeEntry(A, 1,2,2); changeEntry(B, 1,2,0);
   changeEntry(A, 1,3,3); changeEntry(B, 1,3,1);
   changeEntry(A, 2,1,4); changeEntry(B, 2,1,0);
   changeEntry(A, 2,2,5); changeEntry(B, 2,2,1);
   changeEntry(A, 2,3,6); changeEntry(B, 2,3,0);
   changeEntry(A, 3,1,7); changeEntry(B, 3,1,1);
   changeEntry(A, 3,2,8); changeEntry(B, 3,2,1);
   changeEntry(A, 3,3,9); changeEntry(B, 3,3,1);

   printf("%d\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");

   printf("%d\n", NNZ(B));
   printMatrix(stdout, B);
   printf("\n");

   C = scalarMult(1.5, A);
   printf("%d\n", NNZ(C));
   printMatrix(stdout, C);
   printf("\n");

   H = copy(A);
   printf("copy\n");
   printf("%d\n", NNZ(H));
   printMatrix(stdout, H);
   printf("\n");

   F = transpose(B);
   printf("Transpose\n");
   printf("%d\n", NNZ(F));
   printMatrix(stdout, F);
   printf("\n");

   D = sum(A, B);
   printf("Sum\n");
   printf("%d\n", NNZ(D));
   printMatrix(stdout, D);
   printf("\n");

   E = diff(A, A);
   printf("Diff\n");
   printf("%d\n", NNZ(E));
   printMatrix(stdout, E);
   printf("\n");

   G = product(B, B);
   printf("Product\n");
   printf("%d\n", NNZ(G));
   printMatrix(stdout, G);
   printf("\n");


   printf("%s\n", equals(A, H)?"true":"false" );

   makeZero(A);
   printf("Zero\n");
   printf("%d\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");

   freeMatrix(&A);
   freeMatrix(&B);
   freeMatrix(&C);
   freeMatrix(&D);
   freeMatrix(&E);
   freeMatrix(&F);
   freeMatrix(&H);
   freeMatrix(&G);
}