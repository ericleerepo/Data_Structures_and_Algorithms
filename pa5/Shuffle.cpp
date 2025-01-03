#include<iostream>
#include<string>
#include<stdexcept>
#include <fstream>
#include <iomanip>
#include"List.h"

using namespace std;

void shuffle(List& D) {
    // Get the number of elements in D
    int n = D.length();
    if (n <= 1) {
        return;
    }
    //cout << D << endl;
    // Create two temporary Lists to store the two halves
    List firstHalf;
    List secondHalf;

    // Move the cursor to the front and split the List into two halves
    D.moveFront();
    for (int i = 0; i < n / 2; i++) {
        firstHalf.insertBefore(D.moveNext());
    }
    for (int i = n / 2; i < n; i++) {
        secondHalf.insertBefore(D.moveNext());
    }

    //cout << firstHalf << endl;
    //cout << secondHalf << endl;

    // Shuffle the two halves back into D, alternating elements
    D.clear();
    firstHalf.moveFront();
    secondHalf.moveFront();
    while (firstHalf.position() < firstHalf.length() || secondHalf.position() < secondHalf.length()) {
        if (secondHalf.position() < secondHalf.length()) {
            D.insertBefore(secondHalf.moveNext());
        }
        if (firstHalf.position() < firstHalf.length()) {
            D.insertBefore(firstHalf.moveNext());
        }
    }
    //cout<< D << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <max_deck_size>" << endl;
        return 1;
    }

    int maxDeckSize = stoi(argv[1]);
    if (maxDeckSize < 1) {
        cerr << "The maximum deck size must be a positive integer." << endl;
        return 1;
    }

    cout << "deck size       shuffle count" << endl;
    cout << "------------------------------" << endl;

    // Loop through each deck size from 1 to maxDeckSize
    for (int n = 1; n <= maxDeckSize; n++) {
        // Create the original List (0, 1, 2, ..., n-1)
        List deck;
        for (int i = 0; i < n; i++) {
            deck.insertBefore(i);
        }
        //cout <<"deck = " << deck << endl;

        // Shuffle the deck until it returns to the original order
        List originalDeck = deck;
        int shuffleCount = 0;
        do {
            shuffle(deck);
            shuffleCount++;
        } while (!deck.equals(originalDeck));

        // Print the result for the current deck size
        cout << " " << left << setw(16) << n << shuffleCount << endl;
    }                      

    return 0;
} 
