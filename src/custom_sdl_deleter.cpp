#include "custom_sdl_deleter.hpp"

void Custom_SDL_Texture_Deleter::operator()(SDL_Texture *texture) const
{
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Destroying texture...");
	SDL_DestroyTexture(texture);
}

void Custom_SDL_Window_Deleter::operator()(SDL_Window *window) const
{
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Destroying window...");
	SDL_DestroyWindow(window);
}

void Custom_SDL_Renderer_Deleter::operator()(SDL_Renderer *renderer) const
{
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Destroying renderer...");
	SDL_DestroyRenderer(renderer);
}

void Custom_SDL_Surface_Deleter::operator()(SDL_Surface *surface) const
{
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Destroying surface...");
	SDL_FreeSurface(surface);
}