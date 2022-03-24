#include <SDL_image.h>

#include "entity.hpp"

bool Entity::set_entity(shared_renderer_ptr _renderer, const std::string &path)
{
    renderer = _renderer;

    auto surface = unique_surface_ptr(IMG_Load(path.c_str()));
    if (surface == nullptr)
    {
        SDL_Log("Failed to create SDL Surface for img (%s) with error: %s", path.c_str(), IMG_GetError());
        return false;
    }
    // TODO :: Replace with another mechanism
    spriteSrcInTexture = { 0, 0, 21, 16 };
    spriteDstOnSurface = { 0, 0, spriteSrcInTexture.w * 10, spriteSrcInTexture.h * 10 };

    SDL_Log("srcR: %d,%d", spriteSrcInTexture.w, spriteSrcInTexture.h);
    SDL_Log("dstR: %d,%d", spriteDstOnSurface.w, spriteDstOnSurface.h);

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
        SDL_Log("Failed to create scaled SDL Surface for img (%s) with error: %s", path.c_str(), IMG_GetError());
        return false;
    }

    //if (SDL_BlitScaled(surface.get(), nullptr, scaled_surface.get(), &spriteDstOnSurface) < 0)
    if (SDL_BlitScaled(surface.get(), nullptr, scaled_surface.get(), nullptr) < 0)
    {
        SDL_Log("Failed to scale surface: %s", SDL_GetError());
        texture = unique_texture_ptr(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    }
    else
    {
        SDL_Log("Scaling surface by 10X ...");
        texture = unique_texture_ptr(SDL_CreateTextureFromSurface(renderer.get(), scaled_surface.get()));
    }

    // Note: Loading PNG causes "libpng warning: iCCP: known incorrect sRGB profile"
    // https://stackoverflow.com/questions/22745076/libpng-warning-iccp-known-incorrect-srgb-profile
    //texture = unique_texture_ptr(IMG_LoadTexture(renderer.get(), path.c_str()));
    if (texture == nullptr)
    {
        SDL_Log("Failed to create SDL Texture: %s", SDL_GetError());
        return false;
    }

    

    spriteDstOnSurface.x = 720 / 2;
    spriteDstOnSurface.y = 480 / 2;
    //dstR.w = 16;
    //dstR.h = 16;

    return true;
}

bool Entity::set_background(shared_renderer_ptr _renderer, const std::string& path)
{
    renderer = _renderer;

    // Note: Loading PNG causes "libpng warning: iCCP: known incorrect sRGB profile"
    // https://stackoverflow.com/questions/22745076/libpng-warning-iccp-known-incorrect-srgb-profile
    texture = unique_texture_ptr(IMG_LoadTexture(renderer.get(), path.c_str()));
    if (texture == nullptr)
    {
        SDL_Log("Failed to create SDL Texture: %s", SDL_GetError());
        return false;
    }


    return true;
}

shared_renderer_ptr Entity::get_renderer() const
{
    return renderer;
}

shared_texture_ptr Entity::get_texture() const
{
    return texture;
}

bool Entity::render_entity() const
{
    //SDL_RenderCopy(renderer.get(), texture.get(), &srcR, &dstR);
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &spriteDstOnSurface);
    //SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
    return true;
}

bool Entity::render_background() const
{
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
    return true;
}

void Entity::show()
{
    SDL_Log("DSTR: (%d,%d)", spriteDstOnSurface.x, spriteDstOnSurface.y);
}

int Entity::move(int x, int y)
{
    spriteDstOnSurface.x += x;
    spriteDstOnSurface.y += y;
    return 0;
}
