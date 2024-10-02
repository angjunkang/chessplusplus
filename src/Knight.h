#pragma once
#include "Piece.h"

class Knight :
    public Piece
{
public:
    // Constructor
    Knight(Team team, std::pair<int, int> pos, Renderer* handler);

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    virtual Knight* clone() const override {
        return new Knight(*this); // Use copy constructor
    }
};