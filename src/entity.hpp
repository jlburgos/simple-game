#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include "custom_sdl_wrapper.hpp"
#include <string>

class Entity
{
private:
    shared_texture_ptr texture = nullptr;
    shared_renderer_ptr renderer = nullptr;

    // TODO
    SDL_Rect spriteSrcInTexture;
    SDL_Rect spriteDstOnSurface;

public:
    Entity() = default;
    Entity(const Entity&) = default;
    ~Entity() = default;

    bool set_entity(shared_renderer_ptr _renderer, const std::string& path);
    shared_renderer_ptr get_renderer() const;
    shared_texture_ptr get_texture() const;

    bool set_background(shared_renderer_ptr _renderer, const std::string& path);

    int move(int x, int y);
    void show();
    bool render_entity() const;
    bool render_background() const;
};

#endif /* _ENTITY_HPP */