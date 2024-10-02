#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

std::vector<std::tuple<int, int, Piece::MoveType>> Piece::pushMove(std::vector<std::tuple<int, int, Piece::MoveType>> moveList,
																   std::tuple<int, int, Piece::MoveType> move,
																   King* king,
																   Piece* field[8][8],
																   bool checkCheck)
{
	// If move does not put the king in check, add move to list
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		// Check if the king exists before performing further operations
		if (king == nullptr) {
			return moveList;
		}


		bool enemyPlace = true;
		king->setCheck(field, king->getPos().first, king->getPos().second);
		Piece* zwisch = &(*field[std::get<0>(move)][std::get<1>(move)]);
		enemyPlace = false;

		if (field[std::get<0>(move)][std::get<1>(move)] != nullptr)
		{
			enemyPlace = true;
			field[std::get<0>(move)][std::get<1>(move)] = nullptr;
		}

		// Swap current piece with target square to simulate the move
		std::swap(field[std::get<0>(move)][std::get<1>(move)], field[pos.first][pos.second]);
		// If the king is being moved, check the new position of the king if it will lead to a check
		if (type == KING)
		{
			king->setCheck(field, std::get<0>(move), std::get<1>(move));
		}
		// If king is not being moved, check the current position of the king if it will lead to a check
		else
		{
			king->setCheck(field, king->getPos().first, king->getPos().second);
		}

		// Restore state
		std::swap(field[std::get<0>(move)][std::get<1>(move)], field[pos.first][pos.second]);

		if (enemyPlace)
		{
			field[std::get<0>(move)][std::get<1>(move)] = &(*zwisch);
		}
		
		// If king not in check, add the move to the move list
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(field, king->getPos().first, king->getPos().second);
	}
	return moveList;
}

// Get the pointer to the current team king
King* Piece::getOwnKing(Piece* field[8][8])
{
	// Iterate through all squares and check for own team king
	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == team && field[i][j]->getType() == Piece::KING)
				{
					King* ret = static_cast<King*>(field[i][j]);
					return ret;
				}
			}
		}
	}
	return nullptr;
}

Piece::Piece(Team team, std::pair<int, int> pos, Renderer* handler, PieceType type)
	:team(team), pos(pos), m_handler(handler), m_texture(NULL), hasMoved(false), type(type)
{
}

Piece::Piece(const Piece& piece)
	:team(piece.team), pos(piece.pos), m_handler(piece.m_handler), m_texture(NULL), hasMoved(false), type(piece.type)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler->undoPieceRender(pos.first, pos.second);
}

// Method to render the piece to the chessboard
void Piece::render()
{
	SDL_Rect src = { 0, 0, 54, 54 };
	SDL_Rect dest;

	// Calculate the center position of the piece within its board square
	dest.x = m_handler->SCREEN_WIDTH / 8 * pos.first + (m_handler->SCREEN_WIDTH / 8 - 54) / 2;
	dest.y = m_handler->SCREEN_HEIGHT / 8 * pos.second + (m_handler->SCREEN_HEIGHT / 8 - 54) / 2;
	dest.w = 54;
	dest.h = 54; 

	// Render the piece
	if (m_handler != nullptr && m_texture != nullptr)
	{
		m_handler->DrawRectangle(src, dest, m_texture);
	}
	else
	{
		// Handle error or debugging message if handler or texture is null
		std::cout << "Error rendering as texture is null" << std::endl;
	}
}


void Piece::getPieceName()
{
	std::cout << "Empty piece" << std::endl;
}
