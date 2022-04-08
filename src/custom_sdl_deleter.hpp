#ifndef _CUSTOM_SDL_DELETER_HPP
#define _CUSTOM_SDL_DELETER_HPP

#include <SDL.h>

// Create custom deleter classes for SDL objects

class Custom_SDL_Deleter
{
public:
	void operator()(SDL_Texture *texture) const;
	void operator()(SDL_Window *window) const;
	void operator()(SDL_Renderer *renderer) const;
	void operator()(SDL_Surface *surface) const;
};

#endif /* _CUSTOM_SDL_DELETER_HPP */