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

#include "pre-compiled-assets/assets.hpp"
#include "window_manager.hpp"
#include <cstdio>

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * 
 */
WindowManager::WindowManager()
{
}

/**
 * @brief Construct a new Window Manager:: Window Manager object
 * 
 * @param mgr 
 */
WindowManager::WindowManager(const WindowManager &mgr)
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
        fprintf(stderr, "Failed to init SDL_INIT_VIDEO: %s\n", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        fprintf(stderr, "Failed to initialize SDL2_image: %s\n", SDL_GetError());
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
    //surface = SDL_GetWindowSurface(this->window);

    renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        fprintf(stderr, "Failed to create SDL_Renderer! SDL_Error: %s\n", SDL_GetError());
        return 3;
    }

    rw_apple = SDL_RWFromConstMem(APPLE_PNG, APPLE_PNG_LEN);
    rw_plant = SDL_RWFromConstMem(PLANT_JPEG, PLANT_JPEG_LEN);
    surface_apple = IMG_LoadPNG_RW(rw_apple);
    if(surface_apple == nullptr)
    {
        fprintf(stderr, "Failed to create apple surface: %s\n", SDL_GetError());
    }
    surface_plant = IMG_LoadJPG_RW(rw_plant);
    if(surface_plant == nullptr)
    {
        fprintf(stderr, "Failed to create plant surface: %s\n", SDL_GetError());
    }
    texture_apple = SDL_CreateTextureFromSurface(renderer, surface_apple);
    if(texture_apple == nullptr)
    {
        fprintf(stderr, "Failed to create apple texture: %s\n", SDL_GetError());
    }
    texture_plant = SDL_CreateTextureFromSurface(renderer, surface_plant);
    if(texture_plant == nullptr)
    {
        fprintf(stderr, "Failed to create plant texture: %s\n", SDL_GetError());
    }

    return 0;
}

/**
 * @brief 
 * 
 */
void WindowManager::start()
{
    // Fill the surface white
    //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

    bool switch_flag = false;
    while(true)
    {
        SDL_Event e;
        if(SDL_WaitEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                break;
            }
        }
        SDL_RenderClear(renderer);
        if(switch_flag)
        {
            SDL_RenderCopy(renderer, texture_plant, NULL, NULL);
        }
        else
        {
            SDL_RenderCopy(renderer, texture_apple, NULL, NULL);
        }
        switch_flag = !switch_flag;
        SDL_RenderPresent(renderer);
        SDL_Delay(1E3);
    }

    // Update the surface
    //SDL_UpdateWindowSurface(this->window);

    // Wait for a bit
    //SDL_Delay(3E3);
}

/**
 * @brief 
 * 
 */
void WindowManager::close()
{
    // Quit SDL subsystems
    SDL_DestroyTexture(texture_apple);
    SDL_DestroyTexture(texture_plant);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
