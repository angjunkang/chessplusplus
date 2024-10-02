#include "Rook.h"
#include <iostream>

Rook::Rook(Team team, std::pair<int, int> pos, Renderer* handler)
	:Piece(team, pos, handler, ROOK)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/chess_br.png";
	}
	else
	{
		filename = "../res/chess_wr.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void Rook::getPieceName()
{
	if (team == BLACK)
	{
		std::cout << "Black Rook" << std::endl;
	}
	else
	{
		std::cout << "White Rook" << std::endl;
	}
}

void Rook::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;

	int x_copy;
	int y_copy;
	// Loop through x and y directions squares
	for (int x = -1; x <= 1; x ++)
	{
		for (int y = -1; y <= 1; y ++)
		{
			if (y == 0 || x == 0)
			{
				x_copy = x;
				y_copy = y;

				// Check if target square is empty and is in the board
				while (field[pos.first + x_copy][pos.second + y_copy] == nullptr
					&& (pos.first + x_copy >= 0 && pos.first + x_copy <= 7 && pos.second + y_copy >= 0 && pos.second + y_copy <= 7))
				{
					moves = pushMove(moves,
									 std::tuple<int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
									 getOwnKing(field),
									 field,
									 checkCheck);

					// To loop the moves to ensure it goes further along the diagonal
					if (x_copy < 0)
					{
						x_copy -= 1;
					}
					else if (x_copy > 0)
					{
						x_copy += 1;
					}
					if (y_copy < 0)
					{
						y_copy -= 1;
					}
					else if (y_copy > 0)
					{
						y_copy += 1;
					}
				}

				// If there is an enemy piece to be capture at the target spot, push move. No need to iterate along the x and y directions
				if (field[pos.first + x_copy][pos.second + y_copy] != nullptr
					&& (pos.first + x_copy >= 0 && pos.first + x_copy <= 7 && pos.second + y_copy >= 0 && pos.second + y_copy <= 7))
				{
					if (field[pos.first + x_copy][pos.second + y_copy]->getTeam() != team)
					{
						moves = pushMove(moves,
										 std::tuple<int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
										 getOwnKing(field),
										 field,
										 checkCheck);
					}
				}
			}
		}
	}

	possibleMoves = moves;
}