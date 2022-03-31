#ifndef _ASSET_MANAGER_HPP
#define _ASSET_MANAGER_HPP

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
};

#endif /* _ASSET_MANAGER_HPP */