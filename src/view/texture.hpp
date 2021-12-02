/**
 * @file texture.hpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP

#include "../headers.hpp"

class Texture
{
    private:
        SDL_Texture *texture;
    public:
        Texture();
        Texture(std::string filepath);
        ~Texture();
        SDL_Texture *get_texture();
};

#endif /* __TEXTURE_HPP */