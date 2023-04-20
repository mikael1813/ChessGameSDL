#pragma once

#include <SDL.h>
#include <iostream>
#include "ChessWindow.h"

class Application
{
public:
    Application();
    ~Application();

    void loop();
    void draw();
    void mousePress(SDL_MouseButtonEvent& b);
private:
    SDL_Window* m_window;
    SDL_Surface* m_window_surface;
    ChessWindow* chessWindow;
    SDL_Event m_window_event;
};