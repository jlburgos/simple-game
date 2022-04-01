#include "asset_manager.hpp"

AssetManager::AssetManager(const shared_renderer_ptr _renderer) : renderer(_renderer)
{
    assets.push_back(mk_shared_asset_ptr(new Asset(renderer, "blocky-sprite.png")));
    assets.push_back(mk_shared_asset_ptr(new Asset(renderer, "test-background.png")));
}

shared_asset_ptr AssetManager::load_img_asset(const char* filename) {
    // Load dynamic img libs
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initialize SDL Image support: %s", IMG_GetError());
        return nullptr;
    }

    shared_asset_ptr asset = mk_shared_asset_ptr(new Asset(renderer, filename));

    /*
    TODO :: What assets do we load?
    Is this going to be a generic message that you pass stuff into?
    */

    // Unload dynamic img libs
    IMG_Quit();

    return asset;
}