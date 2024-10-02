#include "Game.h"
#include <iostream>
#include <set>

Game::Game(Renderer* handler)
    : wp1(new Pawn(Piece::WHITE, { 0, 6 }, handler)), wp2(new Pawn(Piece::WHITE, { 1, 6 }, handler)),
    wp3(new Pawn(Piece::WHITE, { 2, 6 }, handler)), wp4(new Pawn(Piece::WHITE, { 3, 6 }, handler)),
    wp5(new Pawn(Piece::WHITE, { 4, 6 }, handler)), wp6(new Pawn(Piece::WHITE, { 5, 6 }, handler)),
    wp7(new Pawn(Piece::WHITE, { 6, 6 }, handler)), wp8(new Pawn(Piece::WHITE, { 7, 6 }, handler)),
    bp1(new Pawn(Piece::BLACK, { 0, 1 }, handler)), bp2(new Pawn(Piece::BLACK, { 1, 1 }, handler)),
    bp3(new Pawn(Piece::BLACK, { 2, 1 }, handler)), bp4(new Pawn(Piece::BLACK, { 3, 1 }, handler)),
    bp5(new Pawn(Piece::BLACK, { 4, 1 }, handler)), bp6(new Pawn(Piece::BLACK, { 5, 1 }, handler)),
    bp7(new Pawn(Piece::BLACK, { 6, 1 }, handler)), bp8(new Pawn(Piece::BLACK, { 7, 1 }, handler)),
    wr1(new Rook(Piece::BLACK, { 0, 0 }, handler)), wr2(new Rook(Piece::BLACK, { 7, 0 }, handler)),
    br1(new Rook(Piece::WHITE, { 0, 7 }, handler)), br2(new Rook(Piece::WHITE, { 7, 7 }, handler)),
    wn1(new Knight(Piece::BLACK, { 1, 0 }, handler)), wn2(new Knight(Piece::BLACK, { 6, 0 }, handler)),
    bn1(new Knight(Piece::WHITE, { 1, 7 }, handler)), bn2(new Knight(Piece::WHITE, { 6, 7 }, handler)),
    wb1(new Bishop(Piece::BLACK, { 2, 0 }, handler)), wb2(new Bishop(Piece::BLACK, { 5, 0 }, handler)),
    bb1(new Bishop(Piece::WHITE, { 2, 7 }, handler)), bb2(new Bishop(Piece::WHITE, { 5, 7 }, handler)),
    wq1(new Queen(Piece::BLACK, { 3, 0 }, handler)), bq1(new Queen(Piece::WHITE, { 3, 7 }, handler)),
    wk1(new King(Piece::BLACK, { 4, 0 }, handler)), bk1(new King(Piece::WHITE, { 4, 7 }, handler)),
    turn(Piece::WHITE), m_handler(handler), checkEnPassant(false)
{
    // Initialize field array
    field[0][7] = br1; field[7][7] = br2;
    field[0][0] = wr1; field[7][0] = wr2;
    field[1][7] = bn1; field[6][7] = bn2;
    field[1][0] = wn1; field[6][0] = wn2;
    field[2][7] = bb1; field[5][7] = bb2;
    field[2][0] = wb1; field[5][0] = wb2;
    field[4][7] = bk1; field[4][0] = wk1;
    field[3][7] = bq1; field[3][0] = wq1;
    field[0][1] = bp1; field[1][1] = bp2; field[2][1] = bp3; field[3][1] = bp4;
    field[4][1] = bp5; field[5][1] = bp6; field[6][1] = bp7; field[7][1] = bp8;
    field[0][6] = wp1; field[1][6] = wp2; field[2][6] = wp3; field[3][6] = wp4;
    field[4][6] = wp5; field[5][6] = wp6; field[6][6] = wp7; field[7][6] = wp8;

    // Initialize empty spaces
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            field[j][i] = nullptr;
        }
    }

    // Initialize any other members if needed
}

Game::~Game() {
    // Free dynamically allocated memory
    delete wp1; delete wp2; delete wp3; delete wp4; delete wp5; delete wp6; delete wp7; delete wp8;
    delete bp1; delete bp2; delete bp3; delete bp4; delete bp5; delete bp6; delete bp7; delete bp8;
    delete bn1; delete bn2; delete wn1; delete wn2;
    delete bb1; delete bb2; delete wb1; delete wb2;
    delete wr1; delete wr2; delete br1; delete br2;
    delete wq1; delete bq1;
    delete wk1; delete bk1;
}

Piece* Game::getFieldPos(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;
    return field[row][col];
}

void Game::move(Piece* start, std::tuple<int, int, Piece::MoveType> move)
{
    if (checkEnPassant)
    {
        disableEnPassant();
    }
    else
    {
        checkEnPassant = true;
    }

    switch (std::get<2>(move))
    {
        case Piece::NORMAL:
            normal(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::CASTLE:
            castles(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::ENPASSANT:
            enPassant(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::PROMOTE:
            promote(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        default:
            break;
    }

    gameState();
}


void Game::normal(int xStart, int yStart, int xEnd, int yEnd)
{
    field[xEnd][yEnd] = getFieldPos(xStart, yStart);
    field[xEnd][yEnd]->hasMoved = true;
    field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    field[xEnd][yEnd]->setPosition(std::pair<int, int>(xEnd, yEnd));
    if (field[xEnd][yEnd] != nullptr)
    {
        m_handler->undoPieceRender(xEnd, yEnd);
    }
    field[xEnd][yEnd]->render();

    if (field[xEnd][yEnd]->getType() == Piece::PAWN)
    {
        if (abs(yEnd - yStart) == 2)
        {
            if (xEnd - 1 >= 0)
            {
                if (field[xEnd - 1][yEnd] != nullptr)
                {
                    if (field[xEnd - 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(field[xEnd - 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, -1));
                        checkEnPassant = false;
                    }
                }
            }

            if (xEnd + 1 <= 7)
            {
                if (field[xEnd + 1][yEnd] != nullptr)
                {
                    if (field[xEnd + 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(field[xEnd + 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, 1));
                        checkEnPassant = false;
                    }
                }
            }
        }
    }
}


void Game::enPassant(int xStart, int yStart, int xEnd, int yEnd)
{
    Pawn* pawn_start = static_cast<Pawn*>(field[xStart][yStart]);
    field[xEnd][yEnd - pawn_start->direction] = nullptr;
    field[xEnd][yEnd] = getFieldPos(xStart, yStart);
    field[xEnd][yEnd]->hasMoved = true;
    field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->undoPieceRender(xEnd, yEnd - pawn_start->direction);
    field[xEnd][yEnd]->setPosition(std::pair<int, int>(xEnd, yEnd));
    field[xEnd][yEnd]->render();
}


void Game::promote(int xStart, int yStart, int xEnd, int yEnd)
{
    SDL_Texture* text_rook = m_handler->loadImage("../res/chess_pwr.png");
    SDL_Texture* text_knight = m_handler->loadImage("../res/chess_pwn.png");
    SDL_Texture* text_bishop = m_handler->loadImage("../res/chess_pwb.png");
    SDL_Texture* text_queen = m_handler->loadImage("../res/chess_pwq.png");
    int y_draw = 0;
    Piece::Team team = Piece::WHITE;

    if (field[xStart][yStart]->getTeam() == Piece::BLACK)
    {
        text_rook = m_handler->loadImage("../res/chess_pbr.png");
        text_knight = m_handler->loadImage("../res/chess_pbn.png");
        text_bishop = m_handler->loadImage("../res/chess_pbb.png");
        text_queen = m_handler->loadImage("../res/chess_pbq.png");
        y_draw = 3 * m_handler->SCREEN_HEIGHT / 4;
        team = Piece::BLACK;
    }

    int box_width = m_handler->SCREEN_WIDTH / 4;
    int box_height = m_handler->SCREEN_HEIGHT / 4;
    int icon_size = 72; // Set the size for the icons to 72x72 pixels
    int icon_margin = (box_width - icon_size) / 2;

    SDL_SetRenderDrawColor(m_handler->renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_Rect rectangle = { 0, y_draw, box_width, box_height };
    SDL_RenderFillRect(m_handler->renderer, &rectangle);

    SDL_Rect src = { 0, 0, 72, 72 };
    SDL_Rect dst = { icon_margin, y_draw + icon_margin, icon_size, icon_size };
    m_handler->DrawRectangle(src, dst, text_rook);

    SDL_SetRenderDrawColor(m_handler->renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
    rectangle.x = box_width;
    SDL_RenderFillRect(m_handler->renderer, &rectangle);
    dst.x = box_width + icon_margin;
    m_handler->DrawRectangle(src, dst, text_knight);

    SDL_SetRenderDrawColor(m_handler->renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    rectangle.x = 2 * box_width;
    SDL_RenderFillRect(m_handler->renderer, &rectangle);
    dst.x = 2 * box_width + icon_margin;
    m_handler->DrawRectangle(src, dst, text_bishop);

    SDL_SetRenderDrawColor(m_handler->renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, WHITE_COLOR.a);
    rectangle.x = 3 * box_width;
    SDL_RenderFillRect(m_handler->renderer, &rectangle);
    dst.x = 3 * box_width + icon_margin;
    m_handler->DrawRectangle(src, dst, text_queen);

    bool quit = false;
    Piece* clickedOn = nullptr;

    while (!quit)
    {
        while (SDL_PollEvent(&m_handler->m_event))
        {
            if (m_handler->m_event.type == SDL_QUIT)
            {
                quit = true;
            }

            if (m_handler->m_event.type == SDL_MOUSEBUTTONUP)
            {
                int mouseX = m_handler->m_event.button.x;
                int mouseY = m_handler->m_event.button.y;

                if (mouseY >= y_draw && mouseY < y_draw + box_height)
                {
                    if (mouseX < box_width)
                    {
                        clickedOn = new Rook(team, std::make_pair(xEnd, yEnd), m_handler);
                    }
                    else if (mouseX < 2 * box_width)
                    {
                        clickedOn = new Knight(team, std::make_pair(xEnd, yEnd), m_handler);
                    }
                    else if (mouseX < 3 * box_width)
                    {
                        clickedOn = new Bishop(team, std::make_pair(xEnd, yEnd), m_handler);
                    }
                    else if (mouseX < 4 * box_width)
                    {
                        clickedOn = new Queen(team, std::make_pair(xEnd, yEnd), m_handler);
                    }
                }
            }

            if (m_handler->m_event.type == SDL_MOUSEBUTTONUP && clickedOn != nullptr)
            {
                quit = true;
            }
        }
    }

    field[xEnd][yEnd] = clickedOn;
    field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->renderChessboard();

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] != nullptr)
            {
                field[i][j]->render();
            }
        }
    }

    SDL_DestroyTexture(text_rook);
    SDL_DestroyTexture(text_bishop);
    SDL_DestroyTexture(text_knight);
    SDL_DestroyTexture(text_queen);
}


void Game::castles(int xStart, int yStart, int xEnd, int yEnd)
{
    if (xEnd == 0)
    {
        field[2][yEnd] = field[4][yEnd];
        field[3][yEnd] = field[0][yEnd];
        field[2][yEnd]->hasMoved = true;
        field[3][yEnd]->hasMoved = true;
        field[2][yEnd]->setPosition(std::pair<int, int>(2, yEnd));
        field[3][yEnd]->setPosition(std::pair<int, int>(3, yEnd));
        field[4][yEnd] = nullptr;
        field[0][yEnd] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(0, yEnd);
        field[2][yEnd]->render();
        field[3][yEnd]->render();
    }
    else
    {
        field[6][yEnd] = field[4][yEnd];
        field[5][yEnd] = field[7][yEnd];
        field[6][yEnd]->hasMoved = true;
        field[5][yEnd]->hasMoved = true;
        field[6][yEnd]->setPosition(std::pair<int, int>(6, yEnd));
        field[5][yEnd]->setPosition(std::pair<int, int>(5, yEnd));
        field[4][yEnd] = nullptr;
        field[7][yEnd] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(7, yEnd);
        field[6][yEnd]->render();
        field[5][yEnd]->render();
    }
}

void Game::gameState()
{
    bool lost = true;
    King* pivot = bk1;

    if (turn == Piece::BLACK)
    {
        pivot = wk1;
    }

    pivot->setCheck(field, wk1->getPos().first, wk1->getPos().second);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] != nullptr)
            {
                if (field[i][j]->getTeam() != turn)
                {
                    field[i][j]->getPossibleMoves(field, true);
                    if (!field[i][j]->getPossibleMoves().empty())
                    {
                        lost = false;
                    }
                }
            }
        }
    }

    if (pivot->getCheck() && lost)
    {
        if (turn == Piece::BLACK)
        {
            std::cout << "White wins!";
        }
        else
        {
            std::cout << "Black wins!";
        }
    }
    else if (lost)
    {
        if (turn == Piece::BLACK)
        {
            std::cout << "Draw!";
        }
        else
        {
            std::cout << "Draw!";
        }
    }
    if (turn == Piece::BLACK)
    {
        turn = Piece::WHITE;
    }
    else
    {
        turn = Piece::BLACK;
    }

}


void Game::gameStateLocal() {
    bool lost = true;
    King* pivot = (turn == Piece::BLACK) ? bk1 : wk1;

    pivot->setCheck(field, pivot->getPos().first, pivot->getPos().second);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr && field[i][j]->getTeam() != turn) {
                field[i][j]->getPossibleMoves(field, true);
                if (!field[i][j]->getPossibleMoves().empty()) {
                    lost = false;
                }
            }
        }
    }

    if (pivot->getCheck() && lost) {
        std::cout << ((turn == Piece::BLACK) ? "White wins!" : "Black wins!") << std::endl;
    }
    else if (lost) {
        std::cout << "Draw!" << std::endl;
    }
    else {
        // Switch turn
        turn = (turn == Piece::BLACK) ? Piece::WHITE : Piece::BLACK;

        // If it's the AI's turn, make a move
        if (turn == Piece::BLACK) {
            auto bestMove = getBestMove(Piece::BLACK);
            if (std::get<0>(bestMove) != -1) {
                move(field[std::get<0>(bestMove)][std::get<1>(bestMove)],
                    std::make_tuple(std::get<2>(bestMove), std::get<3>(bestMove), std::get<4>(bestMove)));
            }
        }
    }
}

// Function to evaluate the strength of the board
int evaluateBoard(Piece* field[8][8]) {
    int score = 0;
    // Piece values (ensure these match the type enumeration in your Piece class)
    const int pieceValues[] = { 100, 320, 330, 500, 900, 20000 }; // P, N, B, R, Q, K

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr) {
                int pieceType = field[i][j]->getType();
                int value = pieceValues[pieceType];
                score += (field[i][j]->getTeam() == Piece::WHITE) ? value : -value;
            }
        }
    }

    return score;
}

// Testing method for CPU algorithm 
std::tuple<int, int, int, int, Piece::MoveType> Game::getBestMove(Piece::Team team) {
    // Simple heuristic to get the first valid move
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr && field[i][j]->getTeam() == team) {
                std::vector<std::tuple<int, int, Piece::MoveType>> moves = field[i][j]->getPossibleMoves();
                if (!moves.empty()) {
                    auto move = moves.front();
                    return std::make_tuple(i, j, std::get<0>(move), std::get<1>(move), std::get<2>(move));
                }
            }
        }
    }
    return std::make_tuple(-1, -1, -1, -1, Piece::NORMAL); // No valid moves found
}



void Game::disableEnPassant()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] != nullptr)
            {
                if (field[i][j]->getType() == Piece::PAWN)
                {
                    Pawn* pwn = static_cast<Pawn*>(field[i][j]);
                    pwn->setEnPassant(std::pair<bool, int>(false, 0));
                }
            }
        }
    }
}


void Game::renderPossibleMoves(Piece* piece)
{
    piece->getPossibleMoves(field, true);
    std::vector<std::tuple<int, int, Piece::MoveType>> possible = piece->getPossibleMoves();
    SDL_Rect rectangle;

    // Render the possible moves with highlight textures
    for (const auto& value : possible) {
        int row = std::get<0>(value);
        int col = std::get<1>(value);

        // Determine if there's a piece on the target tile
        SDL_Texture* currentTexture;
        if (field[row][col] != nullptr) {
            // There's a piece on the target tile; use selected texture
            currentTexture = ((row % 2 == 0 && col % 2 == 1) ||
                (row % 2 == 1 && col % 2 == 0)) ?
                m_handler->selectedBlackTexture : m_handler->selectedWhiteTexture;
        }
        else {
            // No piece on the target tile; use possible move texture
            currentTexture = ((row % 2 == 0 && col % 2 == 1) ||
                (row % 2 == 1 && col % 2 == 0)) ?
                m_handler->possibleBlackTexture : m_handler->possibleWhiteTexture;
        }

        rectangle = { row * m_handler->SCREEN_WIDTH / 8,
                      col * m_handler->SCREEN_HEIGHT / 8,
                      m_handler->SCREEN_WIDTH / 8,
                      m_handler->SCREEN_HEIGHT / 8 };

        SDL_RenderCopy(m_handler->renderer, currentTexture, nullptr, &rectangle);
    }

    // Render the board and pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] != nullptr) {
                field[i][j]->render();
            }
        }
    }

    // Get the piece position
    std::pair<int, int> piecePos = piece->getPos();
    int pieceRow = piecePos.first;
    int pieceCol = piecePos.second;

    // Render the selected piece with the selected texture
    SDL_Texture* selectedTexture = ((pieceRow % 2 == 0 && pieceCol % 2 == 1) ||
        (pieceRow % 2 == 1 && pieceCol % 2 == 0)) ?
        m_handler->selectedBlackTexture : m_handler->selectedWhiteTexture;

    rectangle = { pieceRow * m_handler->SCREEN_WIDTH / 8,
                  pieceCol * m_handler->SCREEN_HEIGHT / 8,
                  m_handler->SCREEN_WIDTH / 8,
                  m_handler->SCREEN_HEIGHT / 8 };
    SDL_RenderCopy(m_handler->renderer, selectedTexture, nullptr, &rectangle);

    // Render the piece on top of the selected texture
    Piece* pieceOnSelectedSquare = getFieldPos(pieceRow, pieceCol);
    if (pieceOnSelectedSquare != nullptr) {
        pieceOnSelectedSquare->render();
    }
}



void Game::undoRenderPossibleMoves(Piece* piece)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> possible = piece->getPossibleMoves();

    // Create a set of positions to be cleared
    std::set<std::pair<int, int>> positionsToClear;

    // Add the positions of the possible moves to the set
    for (const auto& value : possible) {
        positionsToClear.insert({ std::get<0>(value), std::get<1>(value) });
    }

    // Add the piece's original position to the set
    positionsToClear.insert(piece->getPos());

    // Render the background for the affected areas
    for (const auto& pos : positionsToClear)
    {
        int i = pos.first;
        int j = pos.second;

        SDL_Texture* currentTexture = ((i + j) % 2 == 0) ? m_handler->whiteSquareTexture : m_handler->blackSquareTexture;

        SDL_Rect rectangle = { i * m_handler->SCREEN_WIDTH / 8,
                              j * m_handler->SCREEN_HEIGHT / 8,
                              m_handler->SCREEN_WIDTH / 8,
                              m_handler->SCREEN_HEIGHT / 8 };

        SDL_RenderCopy(m_handler->renderer, currentTexture, nullptr, &rectangle);
    }

    // Render the pieces involved in the movement
    for (const auto& pos : positionsToClear)
    {
        int i = pos.first;
        int j = pos.second;

        if (field[i][j] != nullptr)
        {
            field[i][j]->render();
        }
    }
}


void Game::getAllPossibleMoves()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] != nullptr)
            {
                field[i][j]->getPossibleMoves(field, true);
            }
        }
    }
}

bool Game::isValidMove(int x, int y, Piece* piece)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> list = piece->getPossibleMoves();
    for (const auto& value : list) {
        if (std::get<0>(value) == x && std::get<1>(value) == y)
        {
            return true;
        }
    }
    return false;
}
