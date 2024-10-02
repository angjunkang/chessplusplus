#include "Renderer.h"
#include <stdio.h>
#include <iostream>

// Initialize method to initialize SDL and create a window.
// Returns true if initialization is successful, false otherwise.
bool Renderer::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow("ChessPlusPlus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Load textures
	whiteSquareTexture = renderTexture("../res/texture_w.png");
	blackSquareTexture = renderTexture("../res/texture_b.png");
	possibleWhiteTexture = renderTexture("../res/texture_dw.png");
	possibleBlackTexture = renderTexture("../res/texture_db.png");
	selectedWhiteTexture = renderTexture("../res/texture_sw.png");
	selectedBlackTexture = renderTexture("../res/texture_sb.png");

	if (whiteSquareTexture == nullptr || blackSquareTexture == nullptr ||
		possibleWhiteTexture == nullptr || possibleBlackTexture == nullptr ||
		selectedWhiteTexture == nullptr || selectedBlackTexture == nullptr) {
		std::cout << "Failed to load some textures!" << std::endl;
		return false;
	}

	return true;
}


SDL_Texture* Renderer::renderTexture(const std::string& path) {
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr) {
			std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}


// Method to render a chessboard pattern on the screen.
// Alternates between whiteColor and blackColor to create a checkerboard effect.
void Renderer::renderChessboard() {
	int squareSize = SCREEN_WIDTH / 8;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			SDL_Texture* currentTexture = ((i + j) % 2 == 0) ? whiteSquareTexture : blackSquareTexture;

			SDL_Rect rectangle = { i * squareSize, j * squareSize, squareSize, squareSize };
			SDL_RenderCopy(renderer, currentTexture, NULL, &rectangle);
		}
	}

	SDL_RenderPresent(renderer);
}

// Load an image file from the specified filename and create an SDL_Texture.
// Returns the created SDL_Texture. If loading fails, outputs an error message.
SDL_Texture* Renderer::loadImage(std::string filename)
{
	SDL_Surface* loadedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());

	if (loadedImage == NULL)
	{
		std::cout << "Error loading " << filename << std::endl;
	}

	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, loadedImage);

	return text;
}




void Renderer::undoPieceRender(int x, int y)
{
	if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1))
	{
		SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
	}
	SDL_Rect rectangle = { x * SCREEN_WIDTH / 8,
						  y * SCREEN_HEIGHT / 8,
						  SCREEN_WIDTH / 8,
						  SCREEN_HEIGHT / 8 };
	SDL_RenderFillRect(renderer, &rectangle);
}

void Renderer::cleanUp()
{
	SDL_DestroyTexture(whiteSquareTexture);
	SDL_DestroyTexture(blackSquareTexture);
	SDL_DestroyTexture(possibleWhiteTexture);
	SDL_DestroyTexture(possibleBlackTexture);
	SDL_DestroyTexture(selectedWhiteTexture);
	SDL_DestroyTexture(selectedBlackTexture);
	SDL_FreeSurface(m_screenSurface);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


void Renderer::DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text)
{
	if (text != nullptr)
	{
		SDL_RenderCopy(renderer, text, &source, &dest);
		SDL_RenderPresent(renderer);

		SDL_UpdateWindowSurface(window);
	}
	else
	{
		std::cout << "DrawRectangle: text was nullptr" << std::endl;
	}
}


Renderer::~Renderer()
{
}
