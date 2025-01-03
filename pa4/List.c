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


// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj* Node;

// private NodeObj type
typedef struct NodeObj{
   ListElement data;
   Node next;
   Node previous;
} NodeObj;

// private QueueObj type
typedef struct ListObj{
   Node front;
   Node back;
   Node cursor;
   int length;
   int index;
} ListObj;


// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(ListElement data){
   Node N = malloc(sizeof(NodeObj));
   assert( N!=NULL );
   N->data = data;
   N->next = N->previous = NULL; //figure out what how to put pointers to correct node
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

// newQueue()
// Returns reference to new empty Queue object.
List newList(){
   List L;
   L = malloc(sizeof(ListObj));
   assert( L!=NULL );
   L->front = L->back = L->cursor= NULL;
   L->length = 0;
   L->index = -1;
   return(L);
}

void freeList(List* pL){
   if(pL!=NULL && *pL!=NULL){ 
      while(length(*pL)>0 ){ 
         deleteBack(*pL); 
      }
      free(*pL);
      *pL = NULL;
   }
}


// Access functions -----------------------------------------------------------

// getFront()
// Returns the value at the front of Q.
// Pre: !isEmpty(Q)
void* front(List L){
   if( L==NULL ){
      fprintf(stderr, "List Error: calling front() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( length(L) == 0 ){
      fprintf(stderr, "List Error: calling front() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   return(L->front->data);
}

void* back(List L){
   if( L==NULL ){
      fprintf(stderr, "List Error: calling front() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if( length(L) == 0 ){
      fprintf(stderr, "List Error: calling front() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   return(L->back->data);
}

// getLength()
// Returns the length of Q.
int length(List L){
   if( L==NULL ){
      fprintf(stderr, "List Error: calling length() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   return(L->length);
}

int index(List L){
   return(L->index);
}

void append(List L, void *x){
   ListElement data = x;
   Node N = newNode(data);
   if (L==NULL){
      fprintf(stderr, "List Error: calling append() on NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if (length(L)==0){
      L->back = N;
      L->front = N;
   }else{
      L->back->next = N;
      N->previous = L->back;
      L->back = N;
   }
   L->length++;
}

void* get(List L){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling get() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L)<0){
      fprintf(stderr, "List Error: calling get() on undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   return L->cursor->data;
}

void moveFront(List L){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling moveFront() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   else{
      L->cursor = L->front;
      L->index = 0;
   }
}

void moveBack(List L){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling moveBack() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   else{
      L->cursor = L->back;
      L->index = length(L)-1;
   }
}

void clear(List L){
   Node clear = L->front;
   while (clear !=NULL){
      Node after = clear->next;
      freeNode(&clear);
      clear = after;
   }
   L->front = NULL;
   L->back = NULL;
   L->cursor = NULL;
   L->length = 0;
   L->index = -1;
}

void set(List L, void *x){
   ListElement data = x;
   if (length(L)==0){
      fprintf(stderr, "List Error: calling get() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L)<0){
      fprintf(stderr, "List Error: calling get() on undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   L->cursor->data = data;
}

void movePrev(List L){
   if(L->index == 0){
      L->cursor = NULL;
      L->index = -1;
   }
   else if(L->index>0){
      L->cursor = L->cursor->previous;
      L->index--;
   }
}

void moveNext(List L){
   if (L == NULL || L->cursor == NULL) {
      return; 
   }
   if(L->index == (L->length-1)){
      L->cursor = NULL;
      L->index = -1;
   }
   else if(L->index < (L->length-1) && L->index>=0){
      L->cursor = L->cursor->next;
      L->index++;
   }
}

void prepend(List L, void *x){
   ListElement data = x;
   Node N = newNode(data);

   if (length(L)==0){
      L->back = N;
      L->front = N;
   }else{
      L->front->previous = N;
      N->next = L->front;
      L->front = N;
   }
   L->length++;
}

void insertBefore(List L, void *x){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling insertBefore() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L)<0){
      fprintf(stderr, "List Error: calling insertBefore() on undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   ListElement data = x;
   Node N = newNode(data);

   N->next = L->cursor;
   N->previous = L->cursor->previous;

   if(L->cursor->previous != NULL){
      L->cursor->previous->next = N;
   } else{
      L->front = N;
   }

   L->cursor->previous = N;

   L->length++;
   L->index++;
}

void insertAfter(List L, void *x){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling insertAfter() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L)<0){
      fprintf(stderr, "List Error: calling insertAfter() on undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   ListElement data = x;
   Node N = newNode(data);

   N->next = L->cursor->next;
   N->previous = L->cursor;

   if(L->cursor->next != NULL){
      L->cursor->next->previous = N;
   } else{
      L->back = N;
   }

   L->cursor->next = N;

   L->length++;
}

void deleteFront(List L){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling deleteFront() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   Node del = L->front;

   if (L->cursor == del) {
        L->cursor = NULL;
        L->index = -1;
   }

   if (L->front == L->back) {
        L->front = NULL;
        L->back = NULL;
   } else {
      L->front = L->front->next;
      L->front->previous = NULL;
   freeNode(&del);
   L->length--;
   }
}

void deleteBack(List L){
   if (length(L)==0){
      fprintf(stderr, "List Error: calling deleteBack() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   //printf("Deleting node from back\n");
   Node del = L->back;

   if (L->cursor == del) {
        L->cursor = NULL;
        L->index = -1;
   }

   if (L->front == L->back) {
        L->front = NULL;
        L->back = NULL;
   } else {
      L->back = L->back->previous;
      if (L->back != NULL) {
         L->back->next = NULL;
      }
   }

   //printf("Freeing node \n");
   freeNode(&del);
   L->length--;
}

void delete(List L){
   if (L == NULL) {
      fprintf(stderr, "List Error: calling delete() on a NULL List reference\n");
      exit(EXIT_FAILURE);
   }
   if (length(L)==0){
      fprintf(stderr, "List Error: calling delete() on an empty List\n");
      exit(EXIT_FAILURE);
   }
   if (index(L)<0){
      fprintf(stderr, "List Error: calling delete() on undefined cursor\n");
      exit(EXIT_FAILURE);
   }
   if (L->cursor == NULL) {
      fprintf(stderr, "List Error: cursor is NULL in delete()\n");
      exit(EXIT_FAILURE);
   }


   if (L->front == L->back) {
        L->front = NULL;
        L->back = NULL;
   } else {
      Node prev = L->cursor->previous;
      Node next = L->cursor->next;

      if (L->cursor == L->front) {
         L->front = next; 
      }

      if (L->cursor == L->back) {
         L->back = prev;  
      }

      if(prev != NULL){
         prev->next = next;
      }
      if(next != NULL){
         next->previous = prev;
      }
   }
   freeNode(&L->cursor);
   L->length--;
   L->cursor = NULL;
   L->index = -1;
}
