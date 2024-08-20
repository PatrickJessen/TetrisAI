#include "Draw.h"
#include "SDL/Render/Renderer.h"
#include <iostream>

Draw::Draw(Board* board)
	:board(board)
{
}

void Draw::drawBoard()
{
    const int width = board->getWidth();
    const int height = board->getHeight();
    const int size = 24;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            SDL_Rect rect = { i * size, j * size, size, size };
            if (board->getGrid()[j][i] == 1) {
                Renderer::SetColor(255, 0, 0, 255);
                SDL_RenderFillRect(Renderer::GetRenderer(), &rect);
            }
            Renderer::SetColor(0, 0, 0, 255);
            SDL_RenderDrawRect(Renderer::GetRenderer(), &rect);
        }
    }
    drawGhostPiece();
    drawNextPieces();
}

void Draw::drawNextPieces()
{
    //int margin = 30;
    //const int size = 24;
    //SDL_Rect squareRect = { 400, 100, 100, 300 };
    //SDL_RenderDrawRect(Renderer::GetRenderer(), &squareRect);
    //for (int i = 0; i < board->getNextShapes().size(); i++) {
    //    for (int j = 0; j < board->getNextShapes()[i]->getShape().size(); j++) {
    //        for (int k = 0; k < board->getNextShapes()[i]->getShape().size(); k++) {
    //            if (board->getNextShapes()[i]->getShape()[k][j] == 1) {
    //                SDL_Rect rect = { 420 + (k * size), (100 + margin) + (j * size), size, size };
    //                Renderer::SetColor(255, 255, 0, 255);
    //                SDL_RenderFillRect(Renderer::GetRenderer(), &rect);
    //            }
    //            /*else {
    //                SDL_Rect rect = { 420 + (k * size), (90) + (j * size), size, size };
    //                Renderer::SetColor(0, 0, 0, 255);
    //                SDL_RenderDrawRect(Renderer::GetRenderer(), &rect);
    //            }*/
    //        }
    //    }
    //    margin += 100;
    //}
}

void Draw::drawGhostPiece()
{
    /*const int size = board->getSize();
    Shape* shape = board->getCurrentShape();
    const int xPos = shape->getXPos();
    const int yPos = shape->getGhostYPos();
    for (int i = 0; i < shape->getShape().size(); ++i) {
        for (int j = 0; j < shape->getShape()[0].size(); ++j) {
            if (shape->getShape()[j][i] == 1) {
                SDL_Rect rect = { (j + xPos) * size, (i + yPos) * size, size, size };
                Renderer::SetColor(0, 255, 0, 255);
                SDL_RenderDrawRect(Renderer::GetRenderer(), &rect);
            }
        }
    }*/
}