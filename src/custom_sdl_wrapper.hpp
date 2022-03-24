#ifndef _CUSTOM_SDL_WRAPPER_HPP
#define _CUSTOM_SDL_WRAPPER_HPP

#include <SDL.h>
#include <SDL_image.h>

#include "custom_sdl_deleter.hpp"
#include "custom_sdl_pointers.hpp"

namespace Custom_SDL_Wrapper
{
	bool init();
	void unload();
}

#endif /* _CUSTOM_SDL_WRAPPER_HPP */