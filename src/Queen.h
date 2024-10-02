#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    // Constructor
    Queen(Team team, std::pair<int, int> pos, Renderer* handler);

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    virtual Queen* clone() const override {
        return new Queen(*this); // Use copy constructor
    }
};

