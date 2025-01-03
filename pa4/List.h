/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA{1} 
* List.h
* .h file for List.cgit s
***/
#ifndef QUEUE_H_INCLUDE_
#define QUEUE_H_INCLUDE_
#include<stdbool.h>


#define FORMAT "%d"  // format string for QueueElement


// Exported types -------------------------------------------------------------
typedef void* ListElement;
typedef struct ListObj* List;


// Constructors-Destructors ---------------------------------------------------

// newQueue()
// Returns reference to new empty Queue object. 
List newList();

// freeQueue()
// Frees all heap memory associated with Queue *pQ, and sets *pQ to NULL.
//void freeList(List* pL);


// Access functions -----------------------------------------------------------

// getFront()
// Returns the value at the front of Q.
// Pre: !isEmpty(Q)
void* front(List L);
void* back(List L);

// getLength()
// Returns the length of Q.
int length(List L);


int index(List L);

void append(List L, void *x);

void prepend(List L, void *x);

void* get(List L);

void moveFront(List L);

void moveBack(List L);

void set(List L, void *x);

void clear(List L);

void movePrev(List L);

void moveNext(List L);

void insertBefore(List L, void *x);

void insertAfter(List L, void *x);

void deleteBack(List L);

void deleteFront(List L);

void delete(List L);

//void printList(FILE* out, List L);

void freeList(List* pL);

#endif