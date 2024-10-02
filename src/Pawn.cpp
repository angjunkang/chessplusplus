#include "Pawn.h"
#include <iostream>
#include <list>

Pawn::Pawn(Team team, std::pair<int, int> pos, Renderer* handler)
	:Piece(team, pos, handler, PAWN), enPassant(std::pair<bool, int>(false, 0))
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/chess_bp.png";
	}
	else
	{
		filename = "../res/chess_wp.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	// Only needed for pawns as pawn only moves in one direction
	if (team == BLACK)
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}

	render();
}

void Pawn::getPieceName()
{
	if (team == BLACK)
	{
		std::cout << "Black Pawn" << std::endl;
	}
	else
	{
		std::cout << "White Pawn" << std::endl;
	}
}

void Pawn::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple<int, int, Piece::MoveType>> moves;

	// Pawn promotion
	if (pos.second + direction == 0 || pos.second + direction == 7)
	{
		if (field[pos.first][pos.second + direction] == nullptr)
		{
			moves = pushMove(moves,
				std::tuple<int, int, Piece::MoveType>(pos.first, pos.second + direction, Piece::PROMOTE),
				getOwnKing(field),
				field,
				checkCheck);
		}
	}
	else
	{
		// Normal forward move: Check if square in front of pawn is empty
		if (field[pos.first][pos.second + direction] == nullptr)
		{
			moves = pushMove(moves,
				std::tuple<int, int, Piece::MoveType>(pos.first, pos.second + direction, Piece::NORMAL),
				getOwnKing(field),
				field,
				checkCheck);
		}
	}


	// Initial double step move: Check if 2 step is within range, is empty and pawn has not moved
	if ((pos.second + 2 * direction >= 0) && (pos.second + 2 * direction <= 7))
	{
		if (field[pos.first][pos.second + 2 * direction] == nullptr && !hasMoved)
		{
			moves = pushMove(moves,
				std::tuple<int, int, Piece::MoveType>(pos.first, pos.second + 2 * direction, Piece::NORMAL),
				getOwnKing(field),
				field,
				checkCheck);
		}
	}

	// Capture diagonally to the right
	if (pos.first + 1 <= 7)
	{
		// Check if diagonal right not empty and has enemy piece
		if (field[pos.first + 1][pos.second + direction] != nullptr)
		{
			if (field[pos.first + 1][pos.second + direction]->getTeam() != team)
			{
				// Check for promotion
				if (pos.second + direction == 0 || pos.second + direction == 7)
				{
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first + 1, pos.second + direction, Piece::PROMOTE),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first + 1, pos.second + direction, Piece::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	// Capture diagonally to the left
	if (pos.first - 1 >= 0)
	{
		// Check if diagonal left not empty and has enemy piece
		if (field[pos.first - 1][pos.second + direction] != nullptr)
		{
			if (field[pos.first - 1][pos.second + direction]->getTeam() != team)
			{
				// Check for promotion
				if (pos.second + direction == 0 || pos.second + direction == 7)
				{
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first - 1, pos.second + direction, Piece::PROMOTE),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						std::tuple<int, int, Piece::MoveType>(pos.first - 1, pos.second + direction, Piece::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	// En passant capture to the right: Check if enpassant is true and the direction
	if (enPassant == std::pair<bool, int>(true, -1))
	{
		moves = pushMove(moves,
			std::tuple<int, int, Piece::MoveType>(pos.first + 1, pos.second + direction, Piece::ENPASSANT),
			getOwnKing(field),
			field,
			checkCheck);
	}

	// En passant capture to the left
	if (enPassant == std::pair<bool, int>(true, 1))
	{
		moves = pushMove(moves,
			std::tuple<int, int, Piece::MoveType>(pos.first - 1, pos.second + direction, Piece::ENPASSANT),
			getOwnKing(field),
			field,
			checkCheck);
	}
	possibleMoves = moves;
}






