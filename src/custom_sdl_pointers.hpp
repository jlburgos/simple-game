#ifndef _CUSTOM_SDL_POINTERS_HPP
#define _CUSTOM_SDL_POINTERS_HPP

#include "custom_sdl_deleter.hpp"

#include <memory>

// Aliases and methods to SDL struct pointers

using unique_window_ptr = std::unique_ptr<SDL_Window, Custom_SDL_Window_Deleter>;

using unique_surface_ptr = std::unique_ptr<SDL_Surface, Custom_SDL_Surface_Deleter>;
using shared_surface_ptr = std::shared_ptr<SDL_Surface>;
std::shared_ptr<SDL_Surface> mk_shared_surface_ptr(SDL_Surface* surface);

using unique_renderer_ptr = std::unique_ptr<SDL_Renderer, Custom_SDL_Renderer_Deleter>;
using shared_renderer_ptr = std::shared_ptr<SDL_Renderer>;
std::shared_ptr<SDL_Renderer> mk_shared_renderer_ptr(SDL_Renderer* renderer);

using unique_texture_ptr = std::unique_ptr<SDL_Texture, Custom_SDL_Texture_Deleter>;
using shared_texture_ptr = std::shared_ptr<SDL_Texture>;
std::shared_ptr<SDL_Texture> mk_shared_texture_ptr(SDL_Texture* texture);

#endif /* _CUSTOM_SDL_POINTERS_HPP */