#include <SDL_image.h>

#include <memory>

#include "asset.hpp"

Asset::Asset(shared_renderer_ptr _renderer, const char* _filename)
{
  (void) _renderer;
  (void) _filename;
  /* TODO: DISABLE FOR NOW
    renderer = _renderer;
    filename = std::make_unique<char[]>(strlen(_filename));
    surface = unique_surface_ptr(IMG_Load(filename));
    if (surface == nullptr)
    {
        SDL_Log("Failed to create Asset for img (%s) with error: %s", filename, IMG_GetError());
        throw AssetException("Failed to load asset");
    }
    texture = mk_shared_texture_ptr(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    */
}

Asset::AssetException::AssetException(const char* msg) : std::runtime_error(msg)
{
    // Nothing to do
}

shared_texture_ptr Asset::get_texture() const
{
    return texture;
}

shared_renderer_ptr Asset::get_renderer() const
{
    return renderer;
}

SDL_Rect Asset::get_src_texture_rect() const
{
    return spriteSrcInTexture;
}

SDL_Rect Asset::get_dst_surface_rect() const
{
    return spriteDstOnSurface;
}

int Asset::scale_texture(const float scale)
{
  (void) scale;
  return 0;
  /* TODO: DISABLE FOR NOW
    auto scaled_surface = unique_surface_ptr(
        SDL_CreateRGBSurface(
            surface->flags,
            spriteDstOnSurface.w,
            spriteDstOnSurface.h,
            surface->format->BitsPerPixel,
            surface->format->Rmask,
            surface->format->Gmask,
            surface->format->Bmask,
            surface->format->Amask
        )
    );
    if (scaled_surface == nullptr)
    {
        SDL_Log("Failed to create scaled SDL Surface with error: %s", IMG_GetError());
        return false;
    }

    //if (SDL_BlitScaled(surface.get(), nullptr, scaled_surface.get(), &spriteDstOnSurface) < 0)
    if (SDL_BlitScaled(surface.get(), nullptr, scaled_surface.get(), nullptr) < 0)
    {
        SDL_Log("Failed to scale surface: %s", SDL_GetError());
        texture = mk_shared_texture_ptr(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    }
    else
    {
        SDL_Log("Scaling surface by %.2f scale", scale);
        texture = mk_shared_texture_ptr(SDL_CreateTextureFromSurface(renderer.get(), scaled_surface.get()));
    }
    */
}

std::shared_ptr<Asset> mk_shared_asset_ptr(Asset* asset) {
	return std::shared_ptr<Asset>(asset);
}
