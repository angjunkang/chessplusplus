#include "Bishop.h"
#include <iostream>

Bishop::Bishop(Team team, std::pair<int, int> pos, Renderer* handler)
    : Piece(team, pos, handler, BISHOP)
{
    std::string filename;
    if (team == BLACK)
    {
        filename = "../res/chess_bb.png";
    }
    else
    {
        filename = "../res/chess_wb.png";
    }
    m_handler = handler;
    m_texture = handler->loadImage(filename);
    render();
}

void Bishop::getPieceName()
{
    if (team == BLACK)
    {
        std::cout << "Black Bishop" << std::endl; // Fixed typo
    }
    else
    {
        std::cout << "White Bishop" << std::endl;
    }
}

void Bishop::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> moves;
    int x_copy;
    int y_copy;

    // Loop through diagonal squares
    for (int x = -1; x <= 1; x += 2)
    {
        for (int y = -1; y <= 1; y += 2)
        {
            x_copy = x;
            y_copy = y;

            // Check if target square is in the board
            while (pos.first + x_copy >= 0 && pos.first + x_copy <= 7 &&
                pos.second + y_copy >= 0 && pos.second + y_copy <= 7 &&
                field[pos.first + x_copy][pos.second + y_copy] == nullptr)
            {
                moves = pushMove(moves,
                    std::tuple<int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
                    getOwnKing(field),
                    field,
                    checkCheck);

                // Move further along the diagonal
                x_copy += (x_copy < 0) ? -1 : 1;
                y_copy += (y_copy < 0) ? -1 : 1;
            }

            // Check for an enemy piece in the next square
            if (pos.first + x_copy >= 0 && pos.first + x_copy <= 7 &&
                pos.second + y_copy >= 0 && pos.second + y_copy <= 7 &&
                field[pos.first + x_copy][pos.second + y_copy] != nullptr &&
                field[pos.first + x_copy][pos.second + y_copy]->getTeam() != team)
            {
                moves = pushMove(moves,
                    std::tuple<int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
                    getOwnKing(field),
                    field,
                    checkCheck);
            }
        }
    }

    possibleMoves = moves;
}
