#pragma once
#include "Piece.h"

class Pawn :
    public Piece
{
public:
    // Constructor
    Pawn(Team team, std::pair<int, int> pos, Renderer* handler);

    // Direction the pawn moves depending on the team
    int direction;

    // Get name of piece
    void getPieceName();

    // Get all possible moves by the piece
    void getPossibleMoves(Piece* field[8][8], bool checkCheck);

    // get the value of enPassant (whether it is activated)
    std::pair<bool, int> getEnPassant() { return enPassant; };

    // set the value of enPassant
    void setEnPassant(std::pair<bool, int> modifi) { enPassant = modifi; };

    virtual Pawn* clone() const override {
        return new Pawn(*this); // Use copy constructor
    }

private:
    // Contains value of enPassant and the direction of the pawn
    std::pair<bool, int> enPassant;
};

