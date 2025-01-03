/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA2
* Graph.h
* Header File for Graph.c
***/ 

#include<stdbool.h>
#include"List.h"

typedef struct MatrixObj* Matrix;

Matrix newMatrix(int n);

int size(Matrix M);

int NNZ(Matrix M);

void makeZero(Matrix M);

void changeEntry(Matrix M, int i, int j, double x);

Matrix copy(Matrix A);

Matrix transpose(Matrix A);

Matrix scalarMult(double x, Matrix A);

Matrix sum(Matrix A, Matrix B);

Matrix diff(Matrix A, Matrix B);

Matrix product(Matrix A, Matrix B);

void freeMatrix(Matrix* pM);

void printMatrix(FILE* out, Matrix M);

int equals(Matrix A, Matrix B);