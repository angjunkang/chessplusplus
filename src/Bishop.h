#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
    // Constructor
    Bishop(Team team, std::pair<int, int> pos, Renderer* handler);

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    virtual Bishop* clone() const override {
        return new Bishop(*this); // Use copy constructor
    }
};
