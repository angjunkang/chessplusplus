#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <string>
#include <vector>

class Renderer
{
public:

	// Fixed screen size
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 640;

	// Fixed colours for chessboard
	const SDL_Color BLACK_COLOR = { 183, 192, 216, 255 };
	const SDL_Color WHITE_COLOR = { 232, 237, 249, 255 };

	// Window to show the application
	SDL_Window* window;

	// Renderer to render items 
	SDL_Renderer* renderer;

	// Surface contained by the window
	SDL_Surface* m_screenSurface;
	
	// Chessboard textures
	SDL_Texture* whiteSquareTexture = nullptr;
	SDL_Texture* blackSquareTexture = nullptr;
	SDL_Texture* possibleWhiteTexture = nullptr;
	SDL_Texture* possibleBlackTexture = nullptr;
	SDL_Texture* selectedWhiteTexture = nullptr;
	SDL_Texture* selectedBlackTexture = nullptr;

	// Event (Mouseclick etc)
	SDL_Event m_event;
	
	// initialize the field
	// returns true, if everything worked
	bool init();

	SDL_Texture* renderTexture(const std::string& path);

	// cleans up SDL stuff
	void cleanUp();

	// destructor
	~Renderer();

	// renders the source rectangle of the texture to dest rectangle
	void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// gets filename, return texture of the jpg file
	SDL_Texture* loadImage(std::string filename);

	//renders the background of the field
	void renderChessboard();
};

