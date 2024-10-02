#ifndef COMPUTER_H
#define COMPUTER_H

#include "Game.h"
#include "Piece.h"

class Computer {
public:
    Computer();
    ~Computer() = default; // Use default destructor since no dynamic memory is managed by the class

private:
    int evaluateBoard(Piece* field[8][8]);
    void copyBoard(Piece* source[8][8], Piece* destination[8][8]);
    bool isGameOver(Piece* field[8][8]);

    static const int pieceValues[6]; // Move pieceValues to a static constant member
};

#endif // COMPUTER_H
