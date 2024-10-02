#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    // Constructor
    King(Team team, std::pair<int, int> pos, Renderer* handler);

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    // To determine if this piece is checked
    void setCheck(Piece* field[8][8], int x, int y);

    // Get the value of check
    bool getCheck() { return check; };

    virtual King* clone() const override {
        return new King(*this); // Use copy constructor
    }

private:
    // Check value if king is under check
    bool check;
};

