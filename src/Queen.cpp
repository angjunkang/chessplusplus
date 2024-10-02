#include "Queen.h"
#include <iostream>

Queen::Queen(Team team, std::pair<int, int> pos, Renderer* handler)
	:Piece(team, pos, handler, QUEEN)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/chess_bq.png";
	}
	else
	{
		filename = "../res/chess_wq.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);
	render();
}


void Queen::getPieceName()
{
	if (team == BLACK)
	{
		std::cout << "Black Queen" << std::endl;
	}
	else
	{
		std::cout << "White Queen" << std::endl;
	}
}

void Queen::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;
	int x_copy;
	int y_copy;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			x_copy = x;
			y_copy = y;
			while (field[pos.first + x_copy][pos.second + y_copy] == nullptr)
			{
				moves = pushMove(moves,
					std::tuple <int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
					getOwnKing(field),
					field,
					checkCheck);
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
			if (field[pos.first + x_copy][pos.second + y_copy] != nullptr
				&& (pos.first + x_copy >= 0 && pos.first + x_copy <= 7 && pos.second + y_copy >= 0 && pos.second + y_copy <= 7))
			{
				if (field[pos.first + x_copy][pos.second + y_copy]->getTeam() != team)
				{
					moves = pushMove(moves,
						std::tuple <int, int, Piece::MoveType>(pos.first + x_copy, pos.second + y_copy, Piece::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	possibleMoves = moves;
}