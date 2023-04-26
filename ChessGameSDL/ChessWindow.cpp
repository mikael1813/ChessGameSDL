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
    initializeToPromote();
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
    if (needToPromote()) {
        if (toPromote.at(i).at(j)) {
            ChessPiece piece;
            int x = 0;
            if (i < game->getSize() / 2) {
                piece.setColor(Color::White);
                i++;
            }
            else {
                piece.setColor(Color::Black);
            }
            piece.setType(promoteOrder.at(abs((game->getSize() / 2) - i)));

            MoveOutcome outcome = game->promote(x, j, piece);

            proccesMoveOutcome(i, j, outcome);

            initializeToPromote();
        }
    }
    else {
        leftClickPressed = true;
        if (lastSelectedPiece.isEmpty()) {
            this->showCircles(i, j);
        }
        else {
            this->removeCircles();

            tuple<unsigned short int, unsigned short int> to{ i,j };

            MoveOutcome outcome = this->game->move(lastSelectedPiece.getData(), to);

            proccesMoveOutcome(i, j, outcome);
        }
    }
}

void ChessWindow::releaseLeftClick() {
    this->leftClickPressed = false;

    tuple<int, int> mousePos = getMousePositions();

    int j = (get<0>(mousePos) - startX) / stepX;
    int i = (get<1>(mousePos) - startY) / stepY;

    this->removeCircles();

    tuple<unsigned short int, unsigned short int> to{ i,j };
    if (lastSelectedPiece.isNotEmpty()) {
        MoveOutcome outcome = this->game->move(lastSelectedPiece.getData(), to);

        proccesMoveOutcome(i, j, outcome);
    }
}

void ChessWindow::proccesMoveOutcome(unsigned short int i, unsigned short int j, MoveOutcome outcome) {
    switch (outcome) {
    case MoveOutcome::InvalidMove:
        this->showCircles(i, j);
        break;
    case MoveOutcome::ValidMove:
        lastSelectedPiece.Empty();
        break;
    case MoveOutcome::WhiteWin:
        //to do
        break;
    case MoveOutcome::BlackWin:
        //to do
        break;
    case MoveOutcome::StaleMate:
        //to do
        break;
    case MoveOutcome::PawnPromote:
        tuple<unsigned short int, unsigned short int> pawn = this->game->getPromotedPawnPositions();
        vector<int> list;
        if (game->getSquare(get<0>(pawn), get<1>(pawn)).getData().getColor() == Color::White) {
            list = vector<int>{ 0,1,2,3 };
        }
        else {
            int size = game->getSize() - 1;
            list = vector<int>{ size,size - 1,size - 2,size - 3 };;
        }
        for (int i : list) {
            toPromote.at(i).at(get<1>(pawn)) = true;
        }
        break;
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
            if (toPromote.at(j).at(i)) {
                SDL_BlitScaled(this->m_white_square, NULL, window_surface, &this->m_image_position);

                ChessPiece piece;
                if (j < this->game->getSize() / 2) {
                    piece.setColor(Color::White);
                    j++;
                }
                else {
                    piece.setColor(Color::Black);
                }

                switch (abs((this->game->getSize() / 2) - j)) {
                case 0:
                    piece.setType(promoteOrder.at(0));
                    break;
                case 1:
                    piece.setType(promoteOrder.at(1));
                    break;
                case 2:
                    piece.setType(promoteOrder.at(2));
                    break;
                case 3:
                    piece.setType(promoteOrder.at(3));
                    break;
                }

                if (piece.getColor() == Color::White) {
                    j--;
                }

                SDL_BlitScaled(pieceImagePaths.at(piece), NULL, window_surface, &this->m_image_position);

            }
            else {
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

                if (this->game->getSquare(j, i).isNotEmpty() && !(leftClickPressed && lastSelectedPiece.isNotEmpty() && get<0>(lastSelectedPiece.getData()) == j && get<1>(lastSelectedPiece.getData()) == i)) {
                    ChessPiece piece = this->game->getSquare(j, i).getData();

                    SDL_BlitScaled(pieceImagePaths.at(piece), NULL, window_surface, &this->m_image_position);
                }

                if (leftClickPressed && lastSelectedPiece.isNotEmpty()) {
                    tuple<int, int> mousePos = getMousePositionsInsideChessBoard();
                    m_image_position.x = get<0>(mousePos);
                    m_image_position.y = get<1>(mousePos);

                    ChessPiece piece = this->game->getSquare(get<0>(lastSelectedPiece.getData()), get<1>(lastSelectedPiece.getData())).getData();

                    SDL_BlitScaled(pieceImagePaths.at(piece), NULL, window_surface, &this->m_image_position);
                }
            }
        }
    }

    //SDL_BlitSurface(this->m_black_square, NULL, window_surface, &this->m_image_position);
}

tuple<int, int> ChessWindow::getMousePositionsInsideChessBoard() {
    int xMouse = 0;
    int yMouse = 0;
    SDL_GetMouseState(&xMouse, &yMouse);
    xMouse = xMouse - stepX / 2;
    yMouse = yMouse - stepY / 2;
    if (xMouse < startX) {
        xMouse = startX;
    }
    if (xMouse > startX + stepX * (this->game->getSize() - 1)) {
        xMouse = startX + stepX * (this->game->getSize() - 1);
    }
    if (yMouse < startY) {
        yMouse = startY;
    }
    if (yMouse > startY + stepY * (this->game->getSize() - 1)) {
        yMouse = startY + stepY * (this->game->getSize() - 1);
    }
    return tuple<int, int>{xMouse, yMouse};
}

tuple<int, int> ChessWindow::getMousePositions() {
    int xMouse = 0;
    int yMouse = 0;
    SDL_GetMouseState(&xMouse, &yMouse);
    if (xMouse < startX) {
        xMouse = startX;
    }
    if (xMouse > startX + stepX * (this->game->getSize()-1)) {
        xMouse = startX + stepX * (this->game->getSize()-1);
    }
    if (yMouse < startY) {
        yMouse = startY;
    }
    if (yMouse > startY + stepY * (this->game->getSize()-1)) {
        yMouse = startY + stepY * (this->game->getSize()-1);
    }
    return tuple<int, int>{xMouse, yMouse};
}
