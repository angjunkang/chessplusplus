#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <stdlib.h>

King::King(Team team, std::pair<int, int> pos, Renderer* handler)
	:Piece(team, pos, handler, KING), check(false)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/chess_bk.png";
	}
	else
	{
		filename = "../res/chess_wk.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void King::getPieceName()
{
	if (team == BLACK)
	{
		std::cout << "Black King" << std::endl;
	}
	else
	{
		std::cout << "White King" << std::endl;
	}
}

void King::getPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;
	bool castles = true;
	bool enemyPlace = false;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (pos.first + x >= 0 && pos.first + x <= 7 && pos.second + y >= 0 && pos.second + y <= 7)
			{
				if (field[pos.first + x][pos.second + y] != nullptr)
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
				else
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

	// Ensure that the king and the rook has not moved for castling
	if (!hasMoved)
	{
		for (int i = 0; i <= 7; i += 7)
		{
			for (int j = 0; j <= 7; j += 7)
			{
				castles = true;
				if (field[i][j] != nullptr)
				{
					if (field[i][j]->getTeam() == team && field[i][j]->getType() == ROOK && !field[i][j]->hasMoved)
					{
						int a, b, c;
						if (i == 0)
						{
							a = 1;
							b = 2;
							c = 3;
						}
						else
						{
							a = 5;
							b = 6;
							c = 6;
						}

						// Check the castling squares are empty and king is not under attack after castling
						if (field[a][j] == nullptr && field[b][j] == nullptr && field[c][j] == nullptr)
						{
							for (int k = 0; k < 8; k++)
							{
								for (int l = 0; l < 8; l++)
								{
									if (field[k][l] != nullptr)
									{
										if (field[k][l]->getTeam() != team)
										{
											std::vector<std::tuple<int, int, Piece::MoveType>> notPossible = field[k][l]->getPossibleMoves();
											for (const auto& value : notPossible)
											{
												if (i == 0)
												{
													if ((std::get<0>(value) == 4 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 2 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 3 && std::get<1>(value) == j))
													{
														castles = false;
													}
												}
												else
												{
													if ((std::get<0>(value) == 5 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 6 && std::get<1>(value) == j)
														|| (std::get<0>(value) == 4 && std::get<1>(value) == j))
													{
														castles = false;
													}
												}
											}
										}
									}
								}
							}
							if (castles)
							{
								moves.push_back(std::tuple<int, int, Piece::MoveType>(i, j, Piece::CASTLE));
							}
						}
					}
				}
			}
		}
	}
	possibleMoves = moves;
}

// Check if the king will be in check, takes in the current field and the current king position
void King::setCheck(Piece* field[8][8], int x, int y) {
	bool isInCheck = false;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (field[i][j] != nullptr) {
				if (field[i][j]->getTeam() != team) {
					// Check if the position is one square away from an enemy king
					if (field[i][j]->getType() == KING) {
						if (abs(field[i][j]->getPos().first - x) <= 1 && abs(field[i][j]->getPos().second - y) <= 1) {
							isInCheck = true;
						}
					}
					// Check if the position is one diagonal square away from an enemy pawn
					else if (field[i][j]->getType() == PAWN) {
						int direction = (field[i][j]->getTeam() == WHITE) ? -1 : 1;
						if ((x == field[i][j]->getPos().first + 1 || x == field[i][j]->getPos().first - 1) &&
							y == field[i][j]->getPos().second + direction) {
							isInCheck = true;
						}
					}
					// Check all possible moves for the rest of the enemy pieces
					else {
						field[i][j]->getPossibleMoves(field, false);
						std::vector<std::tuple<int, int, Piece::MoveType>> possibleMoves = field[i][j]->getPossibleMoves();
						for (const auto& value : possibleMoves) {
							if (std::get<0>(value) == x && std::get<1>(value) == y) {
								isInCheck = true;
							}
						}
					}
				}
			}
		}
	}

	check = isInCheck;

	// Output the check status for the team
	if (check) {
		//std::cout << (team == BLACK ? "Black King is in check!" : "White King is in check!") << std::endl;
	}
	else {
		//std::cout << (team == BLACK ? "Black King is safe." : "White King is safe.") << std::endl;
	}
}
