#include "Application.h"

Application::Application()
{
    m_window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_RESIZABLE);

    if (!m_window)
    {
        std::cout << "Failed to create window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }

    m_window_surface = SDL_GetWindowSurface(m_window);

    if (!m_window_surface)
    {
        std::cout << "Failed to get window's surface\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return;
    }

    this->chessWindow = new ChessWindow();
}

Application::~Application()
{
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
    delete this->chessWindow;
}

void Application::loop()
{
    bool keep_window_open = true;
    while (keep_window_open)
    {
        while (SDL_PollEvent(&m_window_event) > 0)
        {
            switch (m_window_event.type)
            {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousePress(m_window_event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                if (m_window_event.button.button == SDL_BUTTON_LEFT) {
                    this->chessWindow->releaseLeftClick();
                }
                break;
            }
            switch (m_window_event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                m_window_surface = SDL_GetWindowSurface(m_window);
                break;
            }

        }

        //update(1.0 / 60.0);

        draw();
    }
}

void Application::mousePress(SDL_MouseButtonEvent& b) {
    if (b.button == SDL_BUTTON_LEFT) {
        int x = b.x;
        int y = b.y;
        if (chessWindow->isInsideChessBoard(x, y)) {
            chessWindow->onLeftClick(this->m_window_surface, x, y);
        }
    }
    if (b.button)
    if (b.button == SDL_BUTTON_RIGHT) {
        int x = b.x;
        int y = b.y;
    }
}

void Application::draw()
{
    SDL_UpdateWindowSurface(m_window);
    SDL_FillRect(m_window_surface, NULL, SDL_MapRGB(m_window_surface->format, 0, 0, 0));
    this->chessWindow->draw(m_window_surface);
}