#pragma once

#include <SDL.h>

class ChessWindow {
public:
	SDL_Surface* load_surface(char const* path);
	ChessWindow();
	void moveImage(double delta_time);
	void draw(SDL_Surface* window_surface);
private:
	SDL_Surface* m_black_square;
	SDL_Surface* m_white_square;
	SDL_Rect     m_image_position;
	double       m_image_x;
	double       m_image_y;
	const unsigned short board_size = 8;
};