#include "Knight.h"
#include <iostream>

Knight::Knight(Team team, std::pair<int, int> pos, Renderer* handler)
	:Piece(team, pos, handler, KNIGHT)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/chess_bn.png";
	}
	else
	{
		filename = "../res/chess_wn.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);
	render();
}

void Knight::getPieceName()
{
	if (team == BLACK)
	{
		std::cout << "Black Knight" << std::endl;
	}
	else
	{
		std::cout << "White Knight" << std::endl;
	}
}

void Knight::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple<int, int, Piece::MoveType>> moves;
	
	// Move in left/right direction 2 squares
	for (int x = -2; x <= 2; x += 4) 
	{
		for (int y = -1; y <= 1; y += 2)
		{
			// Validate position within board boundaries
			if (pos.first + x >= 0 && pos.first + x <= 7 && pos.second + y >= 0 && pos.second + y <= 7)
			{
				// Check if target square is empty
				if (field[pos.first + x][pos.second + y] == nullptr)
				{
					// Add move as a normal move
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first + x, pos.second + y, Piece::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					// If the piece in the target square is an enemy piece
					if (field[pos.first + x][pos.second + y]->getTeam() != team)
					{
						// Add move as a normal capture move
						moves = pushMove(moves,
							std::tuple<int, int, Piece::MoveType>(pos.first + x, pos.second + y, Piece::NORMAL),
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
			}
		}
	}

	// Move in top/down direction 2 squares
	for (int y = -2; y <= 2; y += 4)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			if (pos.first + x >= 0 && pos.first + x <= 7 && pos.second + y >= 0 && pos.second + y <= 7)
			{
				if (field[pos.first + x][pos.second + y] == nullptr)
				{
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first + x, pos.second + y, Piece::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else if (field[pos.first + x][pos.second + y] != nullptr)
				{
					if (field[pos.first + x][pos.second + y]->getTeam() != team)
					{
						moves = pushMove(moves,
							std::tuple<int, int, Piece::MoveType>(pos.first + x, pos.second + y, Piece::NORMAL),
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