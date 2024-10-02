#pragma once
#include "Piece.h"
#include "Renderer.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

class Game
{
public:
    Game(Renderer* handler);
    ~Game();

    Piece* getFieldPos(int row, int col);
    void move(Piece* piece, std::tuple<int, int, Piece::MoveType> move);
    Piece::Team getTurn() const { return turn; }
    bool isValidMove(int x, int y, Piece* piece);
    void getAllPossibleMoves();
    void renderPossibleMoves(Piece* piece);
    void undoRenderPossibleMoves(Piece* piece);

private:
    Piece* field[8][8];
    void disableEnPassant();
    void normal(int xStart, int yStart, int xEnd, int yEnd);
    void enPassant(int xStart, int yStart, int xEnd, int yEnd);
    void promote(int xStart, int yStart, int xEnd, int yEnd);
    void castles(int xStart, int yStart, int xEnd, int yEnd);
    void gameStateLocal();
    void gameState();
    std::tuple<int, int, int, int, Piece::MoveType> getBestMove(Piece::Team team);

    Piece::Team turn;
    Renderer* m_handler;
    bool checkEnPassant;

    Pawn* wp1; Pawn* wp2; Pawn* wp3; Pawn* wp4; Pawn* wp5; Pawn* wp6; Pawn* wp7; Pawn* wp8;
    Pawn* bp1; Pawn* bp2; Pawn* bp3; Pawn* bp4; Pawn* bp5; Pawn* bp6; Pawn* bp7; Pawn* bp8;
    Knight* bn1; Knight* bn2; Knight* wn1; Knight* wn2;
    Bishop* bb1; Bishop* bb2; Bishop* wb1; Bishop* wb2;
    Rook* wr1; Rook* wr2; Rook* br1; Rook* br2;
    Queen* bq1; Queen* wq1;
    King* bk1; King* wk1;

    const SDL_Color BLACK_COLOR = { 183, 192, 216, 255 };
    const SDL_Color WHITE_COLOR = { 232, 237, 249, 255 };
};
