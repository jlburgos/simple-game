#include "custom_sdl_wrapper.hpp"

bool Custom_SDL_Wrapper::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Failed to initialize SDL support: %s", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initialize SDL Image support: %s", IMG_GetError());
        return false;
    }
    return true;
}

void Custom_SDL_Wrapper::unload()
{
    IMG_Quit();
    SDL_Quit();
}
