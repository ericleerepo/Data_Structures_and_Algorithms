#include<iostream>
#include<string>
#include<stdexcept>
#include"List.h"

using namespace std;

List::Node::Node(ListElement x){
   data = x;
   next = nullptr;
   prev = nullptr;
}

List::List(){
   frontDummy = new Node(0);
   backDummy = new Node(0);
   frontDummy->next = backDummy;
   backDummy->prev = frontDummy;

   beforeCursor = frontDummy;
   afterCursor = backDummy;

   pos_cursor = 0;
   num_elements = 0;
}

List::List(const List& L){
   frontDummy = new Node(0);
   backDummy = new Node(0);
   frontDummy->next = backDummy;
   backDummy->prev = frontDummy;

   beforeCursor = frontDummy;
   afterCursor = backDummy;


   pos_cursor = 0;
   num_elements = 0;

   Node* N = L.frontDummy->next;
   while (N!=L.backDummy){
      insertAfter(N->data);
      moveNext();
      N = N->next;
   }
   //std::cout << "***called copy constructor: " << this->to_string() << std::endl;
}

// Destructor
List::~List(){
   moveFront();
   while(num_elements>0 ){
      eraseAfter();
   }
   delete frontDummy;
   delete backDummy;
}

int List::length() const{
   return(num_elements);
}

ListElement List::front() const{
   if(num_elements==0 ){
      throw std::length_error("List: front(): empty List");
   }
   return(this->frontDummy->next->data);
}

ListElement List::back() const{
   if(num_elements==0 ){
      throw std::length_error("List: back(): empty List");
   }
   return(this->backDummy->prev->data);
}

int List::position() const{
   if(pos_cursor<0 || pos_cursor>length()){
      throw std::range_error("List: position(): invalid cursor");
   }
   return(this->pos_cursor);
}

ListElement List::peekNext() const{
   if(pos_cursor>length()){
      throw std::range_error("List: peekNext(): invalid cursor");
   }
   return (this->afterCursor->data);
}

ListElement List::peekPrev() const{
   if(pos_cursor<=0){
      throw std::range_error("List: peekPrev(): invalid cursor");
   }
   return (this->beforeCursor->data);
}

void List::clear(){
   moveFront();
   while(num_elements>0 ){
      eraseAfter();
   }
   frontDummy->next = backDummy;
   backDummy->prev = frontDummy;
}

void List::insertAfter(ListElement x){
   Node* N = new Node(x);
   if(length()==0){
      frontDummy->next = N;
      backDummy->prev = N;
      N->prev = frontDummy;
      N->next = backDummy;
   }else{
      N->next = afterCursor;
      N->prev = beforeCursor;
      beforeCursor->next = N;
      afterCursor->prev = N;
   }
   afterCursor = N;
   this->num_elements++;
}

void List::insertBefore(ListElement x){
   Node* N = new Node(x);
   if(length()==0){
      frontDummy->next = N;
      backDummy->prev = N;
      N->prev = frontDummy;
      N->next = backDummy;
   }else{
      N->next = afterCursor;
      N->prev = beforeCursor;
      beforeCursor->next = N;
      afterCursor->prev = N;
   }
   beforeCursor = N;
   this->pos_cursor++;
   this->num_elements++;
}

void List::setAfter(ListElement x){
   if(pos_cursor>length()){
      throw std::range_error("List: setAfter(): invalid cursor");
   }
   afterCursor->data = x;
}

void List::setBefore(ListElement x){
   if(pos_cursor<=0){
      throw std::range_error("List: setBefore(): invalid cursor");
   }
   beforeCursor->data = x;
}



ListElement List::moveNext(){
   if(pos_cursor>length()){
      throw std::range_error("List: moveNext(): invalid cursor");
   }
   beforeCursor = afterCursor;
   afterCursor = afterCursor->next;

   // Increment cursor position
   pos_cursor++;

   // Return the data of the element passed over
   return beforeCursor->data;
}

ListElement List::movePrev(){
   if(pos_cursor<=0){
      throw std::range_error("List: movePrev(): invalid cursor");
   }
   afterCursor = beforeCursor;
   beforeCursor = beforeCursor->prev;

   // Increment cursor position
   pos_cursor--;

   // Return the data of the element passed over
   return afterCursor->data;
}

void List::moveFront() {
   // Set beforeCursor to the frontDummy node
   beforeCursor = frontDummy;

   // Set afterCursor to the first real element, or backDummy if the list is empty
   afterCursor = frontDummy->next;

   // Reset cursor position to 0
   pos_cursor = 0;
}

void List::moveBack(){
   beforeCursor = backDummy->prev;
   afterCursor = backDummy;
   pos_cursor = length();
}

void List::eraseAfter() {
   // Check the precondition
   if (pos_cursor >= length()) {
      throw std::range_error("List: eraseAfter(): no element after cursor");
   }

   // Identify the node to be deleted (afterCursor)
   Node* nodeToDelete = afterCursor;

   // Update pointers to bypass the node being deleted
   beforeCursor->next = afterCursor->next;
   afterCursor->next->prev = beforeCursor;

   // Move afterCursor to the next node after the deleted one
   afterCursor = afterCursor->next;

   // Delete the node and free its memory
   delete nodeToDelete;

   // Decrement the number of elements in the list
   num_elements--;
}

void List::eraseBefore(){
   if (pos_cursor <= 0) {
      throw std::range_error("List: eraseBefore(): no element before cursor");
   }
   Node* nodeToDelete = beforeCursor;

   afterCursor->prev = beforeCursor->prev;
   beforeCursor->prev->next = afterCursor;

   beforeCursor = beforeCursor->prev;

   delete nodeToDelete;
   pos_cursor--;
   num_elements--;
}

int List::findNext(ListElement x){
   while(position()<length()){
      if(x==moveNext()){
         return position();
      }
   }
   return -1;
}

int List::findPrev(ListElement x){
   while(position()>0){
      if(x==movePrev()){
         return position();
      }
   }
   return -1;
}

void List::cleanup(){
   int og = beforeCursor->data;
   moveFront();
   ListElement x;
   while(afterCursor!=backDummy){
      x = moveNext();
      int moves = position();
      while(findNext(x) != -1) {  
         eraseBefore();
      }                        
      moveFront();
      for (int i = 0; i < moves; ++i) {
         moveNext();
      }
      
   }
   findNext(og);
}

List List::concat(const List& L) const{
   List J;
   Node* N = this->frontDummy->next;
   Node* M = L.frontDummy->next;
   while( N!=this->backDummy ){
      J.insertBefore(N->data);
      N = N->next;
   }
   while( M!=L.backDummy ){
      J.insertBefore(M->data);
      M = M->next;
   }
   J.moveFront();
   return J;
}


std::string List::to_string() const{
   Node* N = frontDummy->next;
   std::string s = "(";

   while (N!=backDummy){
      s += std::to_string(N->data);
      N = N->next;
      if (N != backDummy) {  
            s += ", ";
      }
   }
   s += ")";
   return s;
}

bool List::equals(const List& R) const{
   bool eq = false;
   Node* N = nullptr;
   Node* M = nullptr;

   eq = ( this->num_elements == R.num_elements );
   N = this->frontDummy->next;
   M = R.frontDummy->next;
   while(eq && N!=this->backDummy){
      eq = (N->data==M->data);
      N = N->next;
      M = M->next;
   }
   return eq;
}

std::ostream& operator<<(std::ostream& stream, const List& L) {
    return stream << L.to_string(); 
}

bool operator==(const List& A, const List& B){
   return A.equals(B);
}

List& List::operator=(const List& L){
   if( this != &L ){
      //clear();
      Node* N = L.frontDummy->next;
      while (N != L.backDummy) {  // Traverse until reaching L's backDummy
         insertAfter(N->data);   // Insert each element after the cursor
         moveNext();             // Move cursor to maintain correct order
         N = N->next;
      }
         // Step 3: Restore the cursor position
      moveFront();
      for (int i = 0; i < L.pos_cursor; ++i) {
         moveNext();
      }
   }
   return *this;
}
