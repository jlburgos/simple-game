#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "asset.hpp"

class AssetManager {
    public:
        AssetManager();
        ~AssetManager();

        int load_img_assets();
        int load_map_assets();

        int unload_img_assets();
        int unload_map_assets();
    private:
};

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() {

}

int AssetManager::load_img_assets() {
    // Load dynamic img libs
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initialize SDL Image support: %s", IMG_GetError());
        return 1;
    }

    /*
    TODO :: What assets do we load?
    Is this going to be a generic message that you pass stuff into?
    */

    // Unload dynamic img libs
    IMG_Quit();
}
