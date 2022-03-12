/**
 * @file texture.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "texture.hpp"

Texture::Texture()
{

}

Texture::Texture(std::string filepath)
{
    (void)filepath;
}

Texture::~Texture()
{
    SDL_DestroyTexture(this->texture);
    this->texture = nullptr;
}

SDL_Texture *Texture::get_texture()
{
    return this->texture;
}