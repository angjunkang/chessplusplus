#pragma once

#include <utility>
#include <vector>
#include "Renderer.h"
#include <SDL.h>
#include <tuple>

class King;

class Piece
{
public:

	enum Team { BLACK, WHITE, NONE };

	enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };

	enum MoveType { NORMAL, CASTLE, ENPASSANT, PROMOTE, INIT };

	// returns list of possible Moves
	std::vector<std::tuple <int, int, Piece::MoveType>> getPossibleMoves() { return possibleMoves; };

	// return whether BLACK or WHITE
	Team getTeam() { return team; };

	// sets new position
	void setPosition(std::pair<int, int> newPos) { pos = newPos; };

	// return position of piece
	std::pair<int, int> getPos() { return pos; };

	// Constructor
	Piece(Team team, std::pair<int,int> pos, Renderer* handler, PieceType type);

	// Copy-Constructor
	Piece(const Piece& piece);

	// Destructor
	~Piece();

	// Render the piece
	void render();

	// Get the name of piece
	virtual void getPieceName();

	// Get all possible moves by the piece
	virtual void getPossibleMoves(Piece* field[8][8], bool checkCheck) = 0;

	// Check if the piece has moved
	bool hasMoved;

	// Get the type of piece
	PieceType getType() { return type; };

	virtual Piece* clone() const = 0;


protected:

	// texture of this piece
	SDL_Texture* m_texture;

	// SDL Handler
	Renderer* m_handler;

	// Team this piece plays for
	Team team;
 
	// The Type of Piece
	PieceType type;

	// List of possible Moves this piece can do <row, col>
	std::vector<std::tuple <int, int, Piece::MoveType>> possibleMoves;
	
	// Position of the piece
	std::pair<int, int> pos;

	// pushes the move, if its allowed.
	// simulates the move, and checks wheter the own king is still checked
	// if king is in check after simulated move, the move is not allowed
	// if checkCheck is true the king simulation will determine whether the move is allowed or not
	// if checkCheck is false, we will just push the move. checkCheck is only false in King's setCheck function,
	// because otherwise it will produce stack overflow (pushMove calls setCheck, setCheck calls pushMove and so on)
	std::vector<std::tuple <int, int, Piece::MoveType>> pushMove(std::vector<std::tuple <int, int, Piece::MoveType>> moveList,
																 std::tuple <int, int, Piece::MoveType> move,
																 King* king,
																 Piece* field[8][8],
																 bool checkCheck);

	// returns king of own team from field
	King* getOwnKing(Piece* field[8][8]);
};

