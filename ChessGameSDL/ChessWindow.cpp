#pragma once

#include <SDL.h>
#include "ChessWindow.h"

ChessWindow::ChessWindow() {
    this->m_black_square = this->load_surface((imageAddress + "black-square.bmp").c_str());
    this->m_white_square = this->load_surface((imageAddress + "white-square.bmp").c_str());
    this->m_small_circle = this->load_surface((imageAddress + "transparent-circle.bmp").c_str());
    this->m_big_circle = this->load_surface((imageAddress + "big-circle.bmp").c_str());

    game = new Game();

    for (int i = 0; i < game->getSize(); i++) {
        for (int j = 0; j < game->getSize(); j++) {
            if (game->getSquare(i, j).isNotEmpty()) {
                if (pieceImagePaths.find(game->getSquare(i, j).getData()) == pieceImagePaths.end()) {
                    ChessPiece piece = this->game->getSquare(i, j).getData();
                    string path = imageAddress;
                    if (piece.getColor() == Color::White) {
                        path += "white-";
                    }
                    else {
                        path += "black-";
                    }
                    if (piece.getType() == Type::Bishop) {
                        path += "bishop";
                    }
                    else if (piece.getType() == Type::Pawn) {
                        path += "pawn";
                    }
                    else if (piece.getType() == Type::Knight) {
                        path += "knight";
                    }
                    else if (piece.getType() == Type::King) {
                        path += "king";
                    }
                    else if (piece.getType() == Type::Queen) {
                        path += "queen";
                    }
                    else {
                        path += "rook";
                    }

                    path += ".bmp";
                    SDL_Surface* m_image_of_piece = this->load_surface(path.c_str());
                    pieceImagePaths.insert({ piece,m_image_of_piece });
                }
            }
        }
    }

    for (int i = 0; i < game->getSize(); i++) {
        std::vector<Circle> row;
        for (int j = 0; j < game->getSize(); j++) {
            row.push_back(Circle::No);
        }
        circled.push_back(row);
    }

    
}

bool ChessWindow::isInsideChessBoard(int x, int y) {
    if (startX <= x && x <= endX && startY <= y && y <= endY) {
        return true;
    }
    return false;
}

SDL_Surface* ChessWindow::load_surface(char const* path){
    SDL_Surface* image_surface = SDL_LoadBMP(path);

    if (!image_surface)
        return 0;

    return image_surface;
}

void ChessWindow::onLeftClick(SDL_Surface* window_surface, int x, int y) {

    int j = (x - startX) / stepX;
    int i = (y - startY) / stepY;
    if (lastSelectedPiece.isEmpty()) {
        this->showCircles(i, j);
    }
    else {
        this->removeCircles();

        tuple<unsigned short int, unsigned short int> to{ i,j };

        if (this->game->move(lastSelectedPiece.getData(), to)) {
            lastSelectedPiece.Empty();
        }
        else {
            this->showCircles(i, j);
        }
    }
}

void ChessWindow::removeCircles() {
    for (int i = 0; i < game->getSize(); i++) {
        for (int j = 0; j < game->getSize(); j++) {
            circled.at(i).at(j) = Circle::No;
        }
    }
}

void ChessWindow::showCircles(int i, int j) {
    Option<ChessPiece> piece = this->game->getSquare(i, j);
    if (piece.isNotEmpty()) {
        lastSelectedPiece.setData(tuple<int, int>{ i, j });
        auto moves = this->game->getAllCorrectMoves(i, j);
        for (tuple<unsigned short int, unsigned short int> t : moves) {
            this->m_image_position.x = startX + stepX * (get<1>(t));
            this->m_image_position.y = startY + stepY * (get<0>(t));
            if (this->game->getSquare(get<0>(t), get<1>(t)).isEmpty()) {
                circled.at(get<1>(t)).at(get<0>(t)) = Circle::Small;
            }
            else {
                circled.at(get<1>(t)).at(get<0>(t)) = Circle::Big;
            }
        }
    }
}

void ChessWindow::draw(SDL_Surface* window_surface) {

    int width = window_surface->w;
    int height = window_surface->h;

    startX = width/2;
    endX = width;
    startY = 0;
    endY = height;

    stepX = (endX - startX) / game->getSize();
    stepY = (endY - startY) / game->getSize();

    if (stepX < stepY) {
        stepY = stepX;
    }
    else {
        stepX = stepY;
    }

    //m_image_position.w = (endX - startX) * (endY - startY) / (board_size * board_size);
    m_image_position.w = stepX;
    m_image_position.h = stepY;

    for (int i = 0; i < game->getSize(); i++) {
        for (int j = 0; j < game->getSize(); j++) {
            m_image_position.x = startX + stepX * i;
            m_image_position.y = startY + stepY * j;
            if ((i + j) % 2 == 0) {
                SDL_BlitScaled(this->m_white_square, NULL, window_surface, &this->m_image_position);
            }
            else {
                SDL_BlitScaled(this->m_black_square, NULL, window_surface, &this->m_image_position);
            }
            if (circled.at(i).at(j) == Circle::Small) {
                SDL_BlitScaled(this->m_small_circle, NULL, window_surface, &this->m_image_position);
            }
            if (circled.at(i).at(j) == Circle::Big) {
                SDL_BlitScaled(this->m_big_circle, NULL, window_surface, &this->m_image_position);
            }

            if (this->game->getSquare(j, i).isNotEmpty()) {
                ChessPiece piece = this->game->getSquare(j, i).getData();

                SDL_BlitScaled(pieceImagePaths.at(piece), NULL, window_surface, &this->m_image_position);
            }
        }
    }

    //SDL_BlitSurface(this->m_black_square, NULL, window_surface, &this->m_image_position);
}
