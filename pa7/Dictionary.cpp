/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA7
* Dictionary.cpp
* Dictionary ADT implementation
***/ 

#include<iostream>
#include<string>
#include<sstream>

#include "Dictionary.h"

using namespace std;

Dictionary::Node::Node(keyType k, valType v){
    key=k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

Dictionary::Dictionary(){
    nil = new Node("-1",-1);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::preOrderCopy(Node* R, Node* N){
    if(R==N || R == nil){
        return;
    }
    setValue(R->key, R->val);
    preOrderCopy(R->left, N);
    preOrderCopy(R->right, N);
}

Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("-1",-1);
    root = nil;
    current = nil;
    num_pairs = 0;

    if(D.root != D.nil){
        preOrderCopy(D.root, D.nil);
    }
}

Dictionary::~Dictionary(){
    clear();
    delete nil;
}

int Dictionary::size() const{
    return num_pairs;
}

void Dictionary::inOrderString(std::string& s, Node* R) const{
    if (R != nil && R!=nullptr){
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s,R->right);
    }
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
    if(R!=nil){
        s += R->key + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::postOrderDelete(Node* R){
    if(R!= nil){
        postOrderDelete(R->left),
        postOrderDelete(R->right);
        delete R;
    }
}

Dictionary::Node* Dictionary::findMin(Node* R){
    if (R == nil){
        return R;
    }
    Node* traveler = R;
    while(traveler->left != nil){
        traveler = traveler->left;
    }
    return traveler;
}

Dictionary::Node* Dictionary::findMax(Node* R){
    if (R == nil){
        return R;
    }
    Node* traveler = R;
    while(traveler->right != nil){
        traveler = traveler->right;
    }
    return traveler;
}

void Dictionary::begin(){
    if (root == nil){
        return;
    }
    Node* traveler = root;
    while(traveler->left != nil){
        traveler = traveler->left;
    }
    current = traveler;
}

void Dictionary::end(){
    if (root == nil){
        return;
    }
    Node* traveler = root;
    while(traveler->right != nil){
        traveler = traveler->right;
    }
    current = traveler;
}

void Dictionary::next(){
    if(!hasCurrent()){
        throw std::logic_error("no current key not found");
    }
    if(current == findMax(root)){
        current = nil;
        return;
    }
    if(current == nil){
        return;
    }
    if(current->right != nil){
        current = findMin(current->right);
        return;
    }else{
        Node* y = current->parent;
        while(y != nil and current == y->right){
            current = y;
            y = y->parent;
        }
        current = y;
    }
}

void Dictionary::prev(){
    if(!hasCurrent()){
        throw std::logic_error("no current key not found");
    }
    if(current == findMin(root)){
        current = nil;
        return;
    }
    if (current->left != nil){
        current = findMax(current->left);
    }else{
        Node* y = current->parent;
        while(y!=nil and current == y->left){
            current = y;
            y = y->parent;
        }
        current = y;
    }
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
    if (R == nil || k == R->key){
        return R;
    }else if (k<R->key){
        return search(R->left, k);
    }else{
        return search(R->right, k);
    }
}

bool Dictionary::contains(keyType k) const{
    if(search(root, k)!= nil){
        return true;
    }
    else{
        return false;
    }
}

valType& Dictionary::getValue(keyType k) const{
    Node* found = search(root,k);
    if(found != nil){
        return found->val;
    }else{
        throw std::logic_error("key not found");
    }
}

bool Dictionary::hasCurrent() const{
    if(current != nil){
        return true;
    }
    else{
        return false;
    }
}

keyType Dictionary::currentKey() const{
    if(!hasCurrent()){
        throw std::logic_error("no current key not found");
    }
    return current->key;
}

valType& Dictionary::currentVal() const{
    if(!hasCurrent()){
        throw std::logic_error("no current val not found");
    }
    return current->val;
}

void Dictionary::clear(){
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v){
    Node* found = search(root, k);
    if(found != nil){
        found->val = v;
    }else{
        Node* insert = new Node(k,v);
        insert->left = nil;
        insert->right = nil;
        Node* y = nil;
        Node* r = root;
        while(r != nil){
            y = r;
            if(k<r->key){
                r = r->left;
            }
            else{
                r = r->right;
            }
        }
        insert->parent = y;
        if(y == nil){
            root = insert;
        }else if(k < y->key){
            y->left = insert;
        }else{
            y->right = insert;
        }
        num_pairs++;
    }
}

void Dictionary::Transplant(Node* u, Node* v){
    if(u->parent == nil){
        root = v;
    }
    else if(u==u->parent->left){
        u->parent->left = v;
    }else{
        u->parent->right = v;
    }
    if(v!=nil){
        v->parent = u->parent;
    }
}

void Dictionary::remove(keyType k){
    Node* del = search(root,k);
    if(del == nil){
       throw std::logic_error("key not found");
    }
    if(del == current){
        current = nil;
    }
    if(del->left == nil){
        Transplant(del, del->right);
    }
    else if(del->right == nil){
        Transplant(del, del->left);
    }else{
        Node* y = findMin(del->right);
        if(y->parent != del){
            Transplant(y, y->right);
            y->right = del->right;
            y->right->parent = y;
        }
        Transplant(del, y);
        y->left = del->left;
        y->left->parent = y;
    }
    delete del;
    num_pairs--;
}

std::string Dictionary::to_string() const{
    string dict;
    inOrderString(dict, root);
    return dict;
}

std::string Dictionary::pre_string() const{
    string dict;
    preOrderString(dict, root);
    return dict;
}

bool Dictionary::equals(const Dictionary& D) const{
    //if(size() != D.size()){
    //    return false;
    //}
    std::string og;
    std::string other;
    inOrderString(og, root);
    inOrderString(other, D.root);
    if (og == other){
        return true;
    }
    return false;
}

std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream<<D.to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

Dictionary& Dictionary::operator=( const Dictionary& D ){
    if (this == &D) {
        return *this; 
    }
    clear();

    if(D.root != D.nil){
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}