#pragma once
#include "Piece.h"

class Rook :
    public Piece
{
public:
    // Constructor
    Rook(Team team, std::pair<int, int> pos, Renderer* renderer);

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    virtual Rook* clone() const override {
        return new Rook(*this); // Use copy constructor
    }
};



