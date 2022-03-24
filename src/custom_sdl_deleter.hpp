#ifndef _CUSTOM_SDL_DELETERS_HPP
#define _CUSTOM_SDL_DELETERS_HPP

#include <SDL.h>

// Create custom deleter classes for SDL objects

class Custom_SDL_Texture_Deleter
{
public:
	void operator()(SDL_Texture *texture) const;
};

class Custom_SDL_Window_Deleter
{
public:
	void operator()(SDL_Window *window) const;
};

class Custom_SDL_Renderer_Deleter
{
public:
	void operator()(SDL_Renderer *renderer) const;
};

class Custom_SDL_Surface_Deleter
{
public:
	void operator()(SDL_Surface *surface) const;
};

#endif /* _CUSTOM_SDL_DELETERS_HPP */