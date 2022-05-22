#ifndef _ASSET_MANAGER_HPP
#define _ASSET_MANAGER_HPP

#include <SDL.h>
#include <SDL_image.h>

#include <vector>

#include "asset.hpp"

class AssetManager {
private:
    shared_renderer_ptr renderer = nullptr;
    std::vector<shared_asset_ptr> assets;
public:
    AssetManager() = delete;
    AssetManager(const shared_renderer_ptr renderer);
    ~AssetManager() = default;

    shared_asset_ptr load_img_asset(const char* filename);
/*
    int load_map_assets();
    int unload_img_assets();
    int unload_map_assets();
*/
};

#endif /* _ASSET_MANAGER_HPP */