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

#include <cstdio>
#include <sstream>

#include "window_manager.hpp"
#include "../util/path.hpp"
#include "../util/logger.hpp"

// Define static members
const std::string WindowManager::DEFAULT_SCREEN_TITLE = "Game Window";
const int WindowManager::DEFAULT_SCREEN_WIDTH = 720;
const int WindowManager::DEFAULT_SCREEN_HEIGHT = 480;
SDL_Window *WindowManager::window = nullptr;
SDL_Renderer *WindowManager::renderer = nullptr;
SDL_Surface *WindowManager::surface_apple = nullptr;
SDL_Surface *WindowManager::surface_plant = nullptr;
SDL_Texture *WindowManager::texture_apple = nullptr;
SDL_Texture *WindowManager::texture_plant = nullptr;

WindowManager::WindowManager()
{
    // Nothing to do
}

WindowManager::~WindowManager()
{
    this->close();
}

int WindowManager::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL support: %s", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initialize SDL Image support: %s", IMG_GetError());
        return 1;
    }
    // Create window
    window = SDL_CreateWindow(DEFAULT_SCREEN_TITLE.c_str(),
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Failed to create SDL_Window: %s", SDL_GetError());
        return 1;
    }
    // Get window surface
    //surface = SDL_GetWindowSurface(this->window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_Log("Failed to create SDL_Renderer: %s", SDL_GetError());
        return 1;
    }

    //rw_apple = SDL_RWFromConstMem(APPLE_PNG, static_cast<int>(APPLE_PNG_LEN));
    //surface_apple = IMG_LoadPNG_RW(rw_apple);

    std::string path = PathNS::get_assets_path() + "/test-imgs/apple.png";
    surface_apple = IMG_Load(path.c_str());
    if(surface_apple == nullptr)
    {
        SDL_Log("Failed to create apple surface: %s", SDL_GetError());
        return 1;
    }

    //rw_plant = SDL_RWFromConstMem(PLANT_JPEG, static_cast<int>(PLANT_JPEG_LEN));
    //surface_plant = IMG_LoadJPG_RW(rw_plant);
    path = PathNS::get_assets_path() + "/test-imgs/plant.jpeg";
    surface_plant = IMG_Load(path.c_str());
    if(surface_plant == nullptr)
    {
        SDL_Log("Failed to create plant surface: %s", IMG_GetError());
        return 1;
    }

    texture_apple = SDL_CreateTextureFromSurface(renderer, surface_apple);
    if(texture_apple == nullptr)
    {
        SDL_Log("Failed to create apple texture: %s", SDL_GetError());
        return 1;
    }

    texture_plant = SDL_CreateTextureFromSurface(renderer, surface_plant);
    if(texture_plant == nullptr)
    {
        SDL_Log("Failed to create plant texture: %s", SDL_GetError());
        return 1;
    }

    IMG_Quit();

    return 0;
}

void WindowManager::start()
{
    // Fill the surface white
    //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
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
        SDL_Log("Before rendering anything, wait a bit...");
        SDL_Delay(1E3);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_Log("Let us initialize renderer color...");
        SDL_Delay(1E3);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture_plant, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Log("Render plant");
        SDL_Delay(1E3);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture_apple, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Log("Render apple");
        SDL_Delay(1E3);

        break; // Avoid infinite loop for now
    }
}

void WindowManager::close()
{
    // Quit SDL subsystems
    SDL_DestroyTexture(texture_apple);
    SDL_DestroyTexture(texture_plant);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
