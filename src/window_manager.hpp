#ifndef _MY_WINDOW_MANAGER_HPP
#define _MY_WINDOW_MANAGER_HPP

/**
 * @file window_manager.hpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <string>
//#include <memory>

#include "headers.hpp"

class WindowManager
{
private:
    static const std::string DEFAULT_SCREEN_TITLE;
    static const int DEFAULT_SCREEN_WIDTH;
    static const int DEFAULT_SCREEN_HEIGHT;
    // std::unique_ptr<SDL_Window> window;
    // std::unique_ptr<SDL_Surface> surface;
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Surface *surface_apple;
    static SDL_Surface *surface_plant;
    static SDL_Texture *texture_apple;
    static SDL_Texture *texture_plant;

public:
    WindowManager();
    ~WindowManager();
    static int init();
    static void start();
    static void close();
};

#endif /* _WINDOW_MANAGER_HPP */