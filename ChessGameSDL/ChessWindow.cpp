#pragma once

#include <SDL.h>
#include "ChessWindow.h"

ChessWindow::ChessWindow() {
    this->m_black_square = this->load_surface("Images/black-square.bmp");
    this->m_white_square = this->load_surface("Images/white-square.bmp");

    m_image_position.x = 0;
    m_image_position.y = 0;
    m_image_position.w = 32;
    m_image_position.h = 32;
    m_image_x = 0.0;
    m_image_y = 0.0;
}

SDL_Surface* ChessWindow::load_surface(char const* path){
    SDL_Surface* image_surface = SDL_LoadBMP(path);

    if (!image_surface)
        return 0;

    return image_surface;
}

void ChessWindow::moveImage(double delta_time) {
    m_image_x = m_image_x + (5 * delta_time);
    m_image_position.x = m_image_x;
}

void ChessWindow::draw(SDL_Surface* window_surface) {

    int width = window_surface->w;
    int height = window_surface->h;

    int startX = width/2;
    int endX = width;
    int startY = 0;
    int endY = height;

    int stepX = (endX - startX) / board_size;
    int stepY = (endY - startY) / board_size;

    if (stepX < stepY) {
        stepY = stepX;
    }
    else {
        stepX = stepY;
    }

    //m_image_position.w = (endX - startX) * (endY - startY) / (board_size * board_size);
    m_image_position.w = stepX;
    m_image_position.h = stepY;

    SDL_Surface* m_white_pawn = this->load_surface("Images/white-pawn.bmp");

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            m_image_position.x = startX + stepX * i;
            m_image_position.y = startY + stepY * j;
            if ((i + j) % 2 == 0) {
                SDL_BlitScaled(this->m_white_square, NULL, window_surface, &this->m_image_position);
            }
            else {
                SDL_BlitScaled(this->m_black_square, NULL, window_surface, &this->m_image_position);
            }
            if (j < 2 || j > 5) {
                SDL_BlitScaled(m_white_pawn, NULL, window_surface, &this->m_image_position);
            }
        }
    }

    //SDL_BlitSurface(this->m_black_square, NULL, window_surface, &this->m_image_position);
}
