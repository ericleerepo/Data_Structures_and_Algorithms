/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA8
* Dictionary.cpp
* Dictionary ADT implementation
***/ 

#include "Dictionary.h"

using namespace std;

Dictionary::Node::Node(keyType k, valType v){
    key=k;
    val = v;
    color = 0;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

Dictionary::Dictionary(){
    nil = new Node("-1",-1);
    nil->left = nil->right = nil;
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::LeftRotate(Node* N){
    Node* y = N->right;

    N->right = y->left;
    if(y->left!=nil){
        y->left->parent = N;
    }
    y->parent = N->parent;
    if (N->parent == nil){ //root parent must be nil!
        root = y;
    }else if (N == N->parent->left){
        N->parent->left = y;
    }else{
        N->parent->right = y;
    }
    y->left = N;
    N->parent = y;
}

void Dictionary::RightRotate(Node* N){
    Node* y = N->left;

    N->left = y->right;
    if(y->right!=nil){
        y->right->parent = N;
    }
    y->parent = N->parent;
    if(N->parent == nil){
        root = y;
    }else if(N == N->parent->right){
        N->parent->right = y;
    }else{
        N->parent->left = y;
    }

    y->right = N;
    N->parent = y;
}

void Dictionary::RB_InsertFixUp(Node* N){
    Node* y;
    while(N->parent->color == 1){
        if(N->parent == N->parent->parent->left){
            y = N->parent->parent->right;
            if(y->color == 1){
                N->parent->color = 0;
                y->color = 0;
                N->parent->parent->color = 1;
                N = N->parent->parent;
            }
            else{
                if(N == N->parent->right){
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = 0;
                N->parent->parent->color = 1;
                RightRotate(N->parent->parent);
            }
        }
        else{
            y = N->parent->parent->left;
            if(y->color == 1){
                N->parent->color = 0;
                y->color=0;
                N->parent->parent->color = 1;
                N = N->parent->parent;
            }
            else{
                if(N==N->parent->left){
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = 0;
                N->parent->parent->color = 1;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = 0;
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
    if (R != nil){
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s,R->right);
    }
}


void Dictionary::debugPrint(Node* node) const {
    if (node == nil) {
        std::cout << "nil\n";
        return;
    }
    std::cout << "Node: " << node->key << ", Val: " << node->val
              << ", Left: " << (node->left == nil ? "nil" : node->left->key)
              << ", Right: " << (node->right == nil ? "nil" : node->right->key)
              << ", Parent: " << (node->parent == nil ? "nil" : node->parent->key)
              << std::endl;
    debugPrint(node->left);
    debugPrint(node->right);
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
    if(R!=nil){
        s += R->key;
        if(R->color == 1){
            s += " (RED)\n";
        }else{
            s +="\n";
        }
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
        Node* y = nil;
        Node* x = root;
        Node* z = new Node(k,v);
        while(x!=nil){
            y = x;
            if(z->key<x->key){
                x = x->left;
            }else{
                x = x->right;
            }
        }
        z->parent = y;
        if(y==nil){
            root = z;
            z->parent = nil;
            z->color = 0;
        }else if(z->key<y->key){
            y->left = z;
        }else{
            y->right = z;
        }
        z->left = nil;
        z->right = nil;
        z->color = 1;
        RB_InsertFixUp(z);
        num_pairs++;
    }
}

void Dictionary::RB_Transplant(Node* u, Node* v){
    if(u->parent == nil){
        root = v;
    }
    else if(u==u->parent->left){
        u->parent->left = v;
    }else{
        u->parent->right = v;
    }
    
        v->parent = u->parent;
    
}

void Dictionary::RB_DeleteFixUp(Node* N){
    Node* w;
    while(N!=root && N->color == 0){
        if(N == N->parent->left){
            w = N->parent->right;
            if(w->color == 1){
                w->color = 0;
                N->parent->color = 1;
                LeftRotate(N->parent);
                w = N->parent->right;
            }
            if(w->left->color == 0 && w->right->color == 0){
                w->color = 1;
                N = N->parent;
            }
            else{
                if(w->right->color == 0){
                    w->left->color = 0;
                    w->color = 1;
                    RightRotate(w);
                    w = N->parent->right;
                }
                w->color = N->parent->color;
                N->parent->color = 0;
                w->right->color = 0;
                LeftRotate(N->parent);
                N = root;
            }
        }
        else{
            w = N->parent->left;
            if(w->color == 1){
                w->color = 0;
                N->parent->color = 1;
                RightRotate(N->parent);
                w = N->parent->left;
            }
            if(w->right->color == 0 && w->left->color == 0){
                w->color = 1;
                N=N->parent;
            }
            else{
                if(w->left->color == 0){
                    w->right->color = 0;
                    w->color = 1;
                    LeftRotate(w);
                    w = N->parent->left;
                }
                w->color = N->parent->color;
                N->parent->color = 0;
                w->left->color = 0;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = 0;
}

void Dictionary::remove(keyType k){
    Node* z = search(root,k);
    if (z == nil) return; 
    Node* y = z;
    Node* x;
    int y_og_color = y->color;
    if(z->left == nil){
        x = z->right;
        RB_Transplant(z, z->right);
    }else if(z->right == nil){
        x = z->left;
        RB_Transplant(z, z->left);
    }else{
        y = findMin(z->right);
        y_og_color = y->color;
        x = y->right;
        if(y->parent == z){
            x->parent = y;
        }else{
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_og_color == 0){
        RB_DeleteFixUp(x);
    }
    delete z;
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
    if(num_pairs!=D.num_pairs){
        cout<<num_pairs;
        return false;
    }
    //debugPrint(root);
    //debugPrint(D.root);
    std::string og = this->to_string();
    std::string other = D.to_string();
    //inOrderString(og, root);
    //inOrderString(other, D.root);
    return (og==other);
}

std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream<<D.to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

Dictionary& Dictionary::operator=( const Dictionary& D ){
    if (this != &D) {
        Dictionary temp = D;
        std::swap(this->nil,temp.nil);
        std::swap(this->root, temp.root);
        std::swap(this->current, temp.current);
        num_pairs = D.num_pairs;
    }
    return *this;
}