/**
 * @file window_manager.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "window_manager.hpp"
#include <cstdio>

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * 
 */
WindowManager::WindowManager() : window(nullptr), surface(nullptr)
{
}

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * 
 * @param mgr 
 */
WindowManager::WindowManager(const WindowManager &mgr) : window(nullptr), surface(nullptr)
{
    // This is not to be used!
    (void)mgr;
}

/**
 * @brief 
 * 
 * @param mgr 
 * @return WindowManager& 
 */
WindowManager& WindowManager::operator=(const WindowManager &mgr)
{
    // This is not to be used!
    (void)mgr;
    return *this;
}

/**
 * @brief Destroy the Window Manager:: Window Manager object
 * 
 */
WindowManager::~WindowManager()
{
    this->close();
}

/**
 * @brief 
 * 
 * @return int 
 */
int WindowManager::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Failed to init SDL_INIT_VIDEO! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create window
    this->window = SDL_CreateWindow(this->DEFAULT_SCREEN_TITLE.c_str(),
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    this->DEFAULT_SCREEN_WIDTH, this->DEFAULT_SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    if (this->window == nullptr)
    {
        fprintf(stderr, "Failed to create SDL_Window! SDL_Error: %s\n", SDL_GetError());
        return 2;
    }
    // Get window surface
    surface = SDL_GetWindowSurface(this->window);

    return 0;
}

/**
 * @brief 
 * 
 */
void WindowManager::start()
{
    // Fill the surface white
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(this->window);

    // Wait two seconds
    SDL_Delay(2000);
    SDL_DestroyWindow(this->window);
}

/**
 * @brief 
 * 
 */
void WindowManager::close()
{
    // Quit SDL subsystems
    SDL_Quit();
}
