#include "custom_sdl_pointers.hpp"

std::shared_ptr<SDL_Window> mk_shared_window_ptr(SDL_Window* window)
{
	return std::shared_ptr<SDL_Window>(window, Custom_SDL_Window_Deleter());
}

std::shared_ptr<SDL_Surface> mk_shared_surface_ptr(SDL_Surface* surface)
{
	return std::shared_ptr<SDL_Surface>(surface, Custom_SDL_Surface_Deleter());
}

std::shared_ptr<SDL_Renderer> mk_shared_renderer_ptr(SDL_Renderer *renderer)
{
	return std::shared_ptr<SDL_Renderer>(renderer, Custom_SDL_Renderer_Deleter());
}

std::shared_ptr<SDL_Texture> mk_shared_texture_ptr(SDL_Texture *texture)
{
	return std::shared_ptr<SDL_Texture>(texture, Custom_SDL_Texture_Deleter());
}

std::shared_ptr<Entity> mk_shared_entity_ptr(Entity* entity)
{
	return std::shared_ptr<Entity>(entity);
}
