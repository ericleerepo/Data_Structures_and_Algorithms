/***
* Eric Lee
* elee144
* 2024 Fall CSE101 PA6 
* BigInteger.cpp
* BigInteger implementation
***/ 

#include<iostream>
#include<string>
#include<stdexcept>
#include<math.h>
#include <sstream>
#include <iomanip>
#include <regex>
#include"BigInteger.h"

using namespace std;

int power = 9;

long base = (long)pow(10,power);

BigInteger::BigInteger(){
    signum = 0;
    digits = List();
}

BigInteger::BigInteger(long x){
    string num = std::to_string(x);
    digits = List();
    if(num[0] == '-'){
        signum = -1;
        num = num.substr(1);
    }else if(x == 0){
        signum = 0;
        return;
    }
    else{
        signum = 1;
    }
    for(int i = num.length()-power ; i>=0; i-=power){
        string part = num.substr(i,power);
        long digit = stol(part);
        digits.insertAfter(digit);
    }
    if (num.length() % power != 0) {
        std::string part = num.substr(0, num.length() % power);
        long digit = std::stol(part);
        digits.insertAfter(digit);
    }
}

BigInteger::BigInteger(std::string s){
    if (s.empty()) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    digits = List();
    if(s[0] == '-'){
        signum = -1;
        s = s.substr(1);
    }else if(s == "0"){
        signum = 0;
        return;
    }
    else if(s[0] == '+'){
        signum = 1;
        s = s.substr(1);
    }
    else{
        signum = 1;
    }
    int non_zero_index = 0;
    while (non_zero_index < int(s.size()) && s[non_zero_index] == '0') {
        non_zero_index++;
    }

    // If the string is empty or only contained zeros
    if (non_zero_index == int(s.size())) {
        signum = 0;  // The number is zero
        digits.insertAfter(0);  // Represent zero in the digits list
        return;
    }

    // Extract the substring without leading zeros
    s = s.substr(non_zero_index);
    if (!std::all_of(s.begin(), s.end(), ::isdigit)) {
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
    }
    for(int i = s.length()-power ; i>=0; i-=power){
        string part = s.substr(i,power);
        long digit = stol(part);
        digits.insertAfter(digit);
    }
    if (s.length() % power != 0) {
        string part = s.substr(0, s.length() % power);
        long digit = stol(part);
        digits.insertAfter(digit);
    }
    
}

BigInteger::BigInteger(const BigInteger& N){
    signum = 0;
    digits = List();
    signum = N.signum;
    digits = N.digits;
}

BigInteger& BigInteger::operator=(const BigInteger& N) {
    if (this != &N) {  // Protect against self-assignment
        signum = N.signum;  // Copy the sign

        // Clear existing digits to prevent old values from persisting
        digits.clear();

        // Copy the digits from N to this object
        List temp = N.digits;
        temp.moveBack();
        while (temp.position() > 0) {
            digits.insertAfter(temp.movePrev());
        }
    }
    return *this;
}

BigInteger::~BigInteger(){
    digits.clear();
}

int BigInteger::sign() const{
    return signum;
}

int BigInteger::compare(const BigInteger& N) const{
    if(signum!=N.signum){
        if(signum == 1 && N.signum == -1){
            return 1;
        }
        if(signum == -1 && N.signum == 1){
            return -1;
        }
    }
    if(signum == 0){
        return 0;
    }
    if (digits.length() != N.digits.length()) {
        if (signum == 1) {
            return (digits.length() < N.digits.length()) ? -1 : 1;
        } else if (signum == -1) {
            return (digits.length() < N.digits.length()) ? 1 : -1;
        }
    }
    List og = digits;
    List comp = N.digits;
    og.moveFront();
    comp.moveFront();
    while(og.position()<og.length()){
        long og_d = og.moveNext();
        long comp_d = comp.moveNext();
        if(og_d != comp_d){
            if (signum == 1) {
                return (og_d < comp_d) ? -1 : 1;
            }else if (signum == -1){
                return (og_d < comp_d) ? 1 : -1;
            }
        }
    }
    return 0;
}

void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}

void BigInteger::negate(){
    if(signum == 1){
        signum = -1;
    }
    else if(signum == -1){
        signum = 1;
    }
}
void negateList(List& L){
    L.moveFront();
    while (L.position() < L.length()) {
        int x = (L.moveNext()*-1);
        L.setBefore(x);
    }
}

int normalizeList(List& L){
    int carry = 0;
    bool neg = false;
    if (L.length() == 0) {
        return 0;
    }
    if(L.front()<0){
      negateList(L);
      neg = true;
    }
    L.moveBack();
    while (L.position() > 0) {
        long i = L.movePrev();
        i += carry; 

        if (i < 0) {
            carry = -1;
            i += base;
        } else {
            carry = i / base;
            i = i % base;
        }
        L.setAfter(i);
    }
    if (L.length() == 0 || (L.length() == 1 && L.front() == 0)) {
        return 0;  
    }
    if(neg){
      return -1;
    }
    if (carry > 0) {
        L.moveFront();
        L.insertAfter(carry);
    }
    return 1;
}

BigInteger BigInteger::add(const BigInteger& N) const{
    if (this->signum == -1 && N.signum == 1) {
        BigInteger absThis = *this;
        absThis.negate(); 
        return N.sub(absThis);
    } else if (this->signum == 1 && N.signum == -1) {
        BigInteger absN = N;
        absN.negate(); 
        return this->sub(absN);
    }
    BigInteger sum;
    List Lone = digits;
    List Ltwo = N.digits;
    Lone.moveFront();
    Ltwo.moveFront();
    while (Lone.length() < Ltwo.length()) {
        Lone.insertAfter(0);
    }
    while (Ltwo.length() < Lone.length()) {
        Ltwo.insertAfter(0);
    }
    Lone.moveBack();
    Ltwo.moveBack();
    while(Lone.position()>0){
        long one = Lone.peekPrev();
        long two = Ltwo.peekPrev();
        long three = one + two;
        sum.digits.insertAfter(three);
        Lone.movePrev();
        Ltwo.movePrev();
    }
    int carry = normalizeList(sum.digits);
    if (this->signum == -1 && N.signum == -1) {
        sum.signum = -1;  
    } else {
        sum.signum = (carry == 0) ? 0 : 1;  
    }
    return sum;
}

bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    }else{
        return false;
    }
}

bool operator<( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B)==-1){
        return true;
    }else{
        return false;
    }
}

bool operator>(const BigInteger& A, const BigInteger& B){
    if(A.compare(B)==1){
        return true;
    }
    else{
        return false;
    }
}

bool operator<=( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B)==0 || A.compare(B)==-1){
        return true;
    }
    else{
        return false;
    }
}

bool operator>=( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B)==0 || A.compare(B)==1){
        return true;
    }
    else{
        return false;
    }
}

BigInteger BigInteger::sub(const BigInteger& N) const{
    BigInteger sub;
    if (N.signum == 0) {
        return *this; 
    }
    if (this->signum == 0) {
        sub = N;
        sub.signum *= -1;
        return sub;
    }
    if(this->sign() == 1 && N.sign() == 1 && this->compare(N)==0){
        sub.signum = 0;
        return sub;
    }
    if (this->signum == -1 && N.signum == 1){
        // (-this) - N == -(this + N)
        BigInteger absA = BigInteger(*this);
        absA.negate();
        sub = absA.add(N);
        sub.signum = -1;
        return sub;
    }
    else if (this->signum == -1 && N.signum == -1){
        // (-A) - (-B) == B - A
        BigInteger absA = BigInteger(*this);
        absA.negate();
        BigInteger absN = BigInteger(N);
        absN.negate();

        if (absA.compare(absN) ==1) {
            // |A| >= |B| => -(A - B)
            sub = absA.sub(absN);
            sub.signum = -1; // Result is negative
        } else if(absA.compare(absN) ==0){
            sub.signum = 0;
            return sub;
        }
        else {
            // |A| < |B| => B - A
            sub = absN.sub(absA);
            sub.signum = 1; // Result is positive
        }
        return sub;
    }else if(this->signum == 1 && N.signum == -1){
        BigInteger absN = BigInteger(N);
        absN.negate();
        return this->add(absN);
    }
    List Lone = digits;
    List Ltwo = N.digits;
    Lone.moveFront();
    Ltwo.moveFront();
    while (Lone.length() < Ltwo.length()) {
        Lone.insertAfter(0);
    }
    while (Ltwo.length() < Lone.length()) {
        Ltwo.insertAfter(0);
    }
    Lone.moveBack();
    Ltwo.moveBack();
    while(Lone.position()>0){
        long one = Lone.movePrev();
        long two = Ltwo.movePrev();
        long three = one - two;
        sub.digits.insertAfter(three);
    }
    int carry = normalizeList(sub.digits);
    sub.signum = carry;
    return sub;
}

void scalarMultList(List& L, ListElement m){
    L.moveFront();
    while(L.position()<L.length()){
        long x = L.moveNext();
        x = x*m;
        L.setBefore(x);
    }
}

void shiftList(List& L, int p){
    L.moveBack();
    for(int i = 0; i<p; i++){
        L.insertAfter(0);
    }
}

BigInteger BigInteger::mult(const BigInteger& N) const{

    BigInteger product;
    product.signum = this->signum * N.signum;
    if (product.signum == 0){
        return product;
    }
    BigInteger temp = BigInteger();
    BigInteger M;
    temp.signum = 1;
    List Lone = digits;
    List Ltwo = N.digits;
    Ltwo.moveBack();
    int shift = 0;
    while(Ltwo.position()>0){
        long scalar = Ltwo.movePrev();
        List Lone_copy = List(Lone);
        scalarMultList(Lone_copy, scalar);
        shiftList(Lone_copy, shift);
        temp.digits = Lone_copy;
        M = product.add(temp);
        product.makeZero();
        product = M;
        shift++;
        temp.makeZero();
        M.makeZero();
    }
    product.signum = this->signum * N.signum;
    return product;
}

std::string BigInteger::to_string(){
    string num;
    if(signum == -1){
        num = num.append("-");
    }
    else if(signum == 0){
        return "0";
    }
    for(digits.moveFront(); digits.position()<digits.length(); digits.moveNext()){
        long digit = digits.peekNext();
        string str = std::to_string(digit);
        if (num.empty() || num == "-") {
            if (str !="0"){
                num.append(str);
            }
        }
        else{
            std::ostringstream format;
            format << std::setw(power) << std::setfill('0') << digit; // Assume power = 3
            num.append(format.str());
        }
    }
    if (num == "-0"){
        return "0";
    }
    return num;
}

BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
   BigInteger M = A;
   A.makeZero();
   A = M.add(B);
   return A;
}
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
   BigInteger M = A;
   A.makeZero();
   A = M.sub(B);
   return A;
}
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
   BigInteger M = A;
   A.makeZero();
   A = M.mult(B);
   return A;
}
std::ostream& operator<<(std::ostream& stream, BigInteger N) {
    return stream<< N.to_string();
}