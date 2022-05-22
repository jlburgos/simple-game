#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include "custom_sdl_wrapper.hpp"
#include "asset.hpp"
#include <string>

class Entity
{
private:
    shared_asset_ptr asset = nullptr;
    SDL_Rect spriteSrcInTexture;
    SDL_Rect spriteDstOnSurface;
    shared_renderer_ptr renderer = nullptr;
    shared_texture_ptr texture = nullptr;

public:
    Entity() = default;
    Entity(const Entity& entity);
    ~Entity() = default;

    bool set_entity(shared_renderer_ptr _renderer, const std::string& path);
    shared_renderer_ptr get_renderer() const;
    shared_texture_ptr get_texture() const;
    SDL_Rect get_src_texture() const;
    SDL_Rect get_dst_texture() const;

    bool set_background(shared_renderer_ptr _renderer, const std::string& path);

    int move(int x, int y);
    void show();
    bool render_entity() const;
    bool render_background() const;

    //virtual bool render() = 0;
};

using shared_entity_ptr = std::shared_ptr<Entity>;
std::shared_ptr<Entity> mk_shared_entity_ptr(Entity* entity);

#endif /* _ENTITY_HPP */
