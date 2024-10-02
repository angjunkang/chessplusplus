#include <SDL.h>
#include <memory>
#include "GameLoop.h"
#include "Renderer.h"
#include "Game.h"
#include <vector>
#include <tuple>

const int TILE_SIZE = 80; // Define the tile size for the board

void GameLoop::run()
{
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    renderer->init();

    bool quit = false;

    renderer->renderChessboard(); // Show chessboard now that 2 clients are connected

    std::unique_ptr<Game> game = std::make_unique<Game>(renderer.get());

    Piece* clickedOn = nullptr;
    int xStart = -1;
    int yStart = -1;
    int xEnd = -1;
    int yEnd = -1;

    // Game loop to keep the game running until quit
    while (!quit)
    {
        while (SDL_PollEvent(&renderer->m_event))
        {
            // Quit event
            if (renderer->m_event.type == SDL_QUIT)
            {
                quit = true;
            }

            // Mouse click down event
            if (renderer->m_event.type == SDL_MOUSEBUTTONDOWN)
            {
                xStart = renderer->m_event.button.x / TILE_SIZE;
                yStart = renderer->m_event.button.y / TILE_SIZE;
                clickedOn = game->getFieldPos(xStart, yStart);
                if (clickedOn != nullptr)
                {
                    if (clickedOn->getTeam() == game->getTurn())
                    {
                        game->renderPossibleMoves(clickedOn);
                    }
                }
            }

            // Mouse click up event
            if (renderer->m_event.type == SDL_MOUSEBUTTONUP)
            {
                if (clickedOn != nullptr)
                {
                    if (clickedOn->getTeam() == game->getTurn())
                    {
                        game->undoRenderPossibleMoves(clickedOn);
                    }
                }
                xEnd = renderer->m_event.button.x / TILE_SIZE;
                yEnd = renderer->m_event.button.y / TILE_SIZE;
                if (clickedOn != nullptr)
                {
                    if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
                        && (clickedOn->getTeam() == game->getTurn())
                        && (game->isValidMove(xEnd, yEnd, clickedOn)))
                    {
                        std::vector<std::tuple<int, int, Piece::MoveType>> list = game->getFieldPos(xStart, yStart)->getPossibleMoves();
                        for (const auto& value : list)
                        {
                            if (std::get<0>(value) == xEnd && std::get<1>(value) == yEnd)
                            {
                                game->move(clickedOn, std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, std::get<2>(value)));
                            }
                        }
                        xStart = -1;
                        yStart = -1;
                        xEnd = -1;
                        yEnd = -1;
                        game->getAllPossibleMoves();
                        clickedOn = nullptr;
                    }
                }
            }
        }
    }

    // Clean up
    renderer->cleanUp();
}
