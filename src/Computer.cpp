#include "Computer.h"
#include <climits>
#include <iostream>

// Initialize the static piece values array
const int Computer::pieceValues[6] = { 100, 320, 330, 500, 900, 20000 };

// Constructor
Computer::Computer() {}

// Function to evaluate the strength of the board
int Computer::evaluateBoard(Piece* field[8][8]) {
    int score = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr) {
                int pieceType = field[i][j]->getType();
                int value = pieceValues[pieceType];
                score += (field[i][j]->getTeam() == Piece::WHITE) ? -value : value;
            }
        }
    }

    return score;
}

void Computer::copyBoard(Piece* source[8][8], Piece* destination[8][8]) {
    // Deallocate existing pieces in the destination array to prevent memory leaks
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (destination[i][j] != nullptr) {
                delete destination[i][j];
                destination[i][j] = nullptr;
            }
        }
    }

    // Perform the deep copy
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (source[i][j] != nullptr) {
                destination[i][j] = source[i][j]->clone();
            }
        }
    }
}

bool Computer::isGameOver(Piece* field[8][8]) {
    bool whiteKingAlive = false;
    bool blackKingAlive = false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr && field[i][j]->getType() == Piece::KING) {
                if (field[i][j]->getTeam() == Piece::WHITE) whiteKingAlive = true;
                if (field[i][j]->getTeam() == Piece::BLACK) blackKingAlive = true;
            }
        }
    }

    return !whiteKingAlive || !blackKingAlive;
}
